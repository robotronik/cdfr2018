#include "camera.h"

static int camera_pid = -1;
static int camera_socket = -1;
static bool read_started = false;
static int start_date = 0;

int Start_Camera(){
  if(camera_pid != -1){
    log_vwarning("Camera script were already running (%d). Restarting.", camera_pid);
    Stop_Camera();
  }
  
  //unlink camera_socket
  int unlink_pid = fork();
  if(!unlink_pid){
    execlp("unlink", "unlink", CAMERA_SOCKET, NULL);
  }else if(unlink_pid != -1){
    waitpid(unlink_pid, NULL, 0);
  }else{
    log_warning("Could not unlink camera socket.");
  }
  
  //Socket
  camera_socket = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if(camera_socket == -1){
    log_verror("Camera : failed to create socket : %s", STR_ERRNO);
    return -1;
  }
  
  struct sockaddr_un sockaddr = {.sun_family = AF_UNIX,
				 .sun_path = CAMERA_SOCKET};
  socklen_t addrlen = sizeof(sockaddr);

  //Binding socket
  if(bind(camera_socket, (const struct sockaddr*) &sockaddr, addrlen) == -1){
    log_verror("Camera : bind socket failed : %s", STR_ERRNO);
    return -1;
  }

  //Listening
  if(listen(camera_socket, 1) == -1){
    log_verror("Camera : setting socket as passive failed : %s", STR_ERRNO);
  }

  //Starting python script
  camera_pid = fork();
  if(camera_pid == -1){
    log_verror("Camera : fork failed : %s", STR_ERRNO);
    return -1;
  }

  if(camera_pid == 0){
    if(execlp("python3", "python3", CAMERA_PATH, VIDEO_PATH, CAMERA_SOCKET, NULL) == -1){
      log_verror("Camera : exec failed : %s", STR_ERRNO);
      exit(EXIT_FAILURE);
      do{}while(1);
    }
  }

  log_vinfo("Camera script started (pid %d)", camera_pid);

  //Waiting for connection
  int start = get_time_ms();
  int r;
  do{
    r = accept(camera_socket, (struct sockaddr*) &sockaddr, &addrlen);
  }while(r == -1 && errno == EAGAIN && get_time_ms() - start < 5000);
  if(r == -1){
    log_error("Camera : accept failed : timeout reached");
    return -1;
  }else{
    camera_socket = r;
    log_info("Socket connection successful");
  }
  
  return 0;
}

CV_State Read_Plan(char colors[4]){
  if(camera_pid == -1){
    log_warning("Read_Plan called before Start_Camera. Calling Start_Camera().");
    Start_Camera();
  }

  int count;
  if(read_started){
    if(fcntl(camera_socket, F_SETFL, O_NONBLOCK) == -1){
      log_verror("fcntl failed : %s", STR_ERRNO);
    }
    
    count = read(camera_socket, colors, 4);
    colors[3] = '\0';
    
    if(count == -1 && errno == EWOULDBLOCK){
      if(get_time_ms() - start_date > CV_TIMEOUT){
	read_started = false;
	log_error("Read plan timed out");
	return CV_ERR_TIMEOUT;
      }
    }else{
      read_started = false;
      if(colors[0] == '-'){
	log_info("No plan found on image");
	return CV_ERR;
      }else{
	log_vinfo("Received plan : %s", colors);
	return CV_OK;
      }
    }
  }else{
    if(write(camera_socket, "read", 5) == -1 && errno != EWOULDBLOCK){
      log_verror("Write on camera socket failed : %s", STR_ERRNO);
      return CV_ERR;
    }
    colors[0] = colors[1] = colors[2] = '-';
    colors[3] = '\0';
    read_started = true;
    start_date = get_time_ms();
  }

  return CV_RUNNING;
}

void Stop_Camera(){
  if(camera_pid != -1){
    if(kill(camera_pid, SIGTERM) == -1){
      log_vwarning("Player (pid %d) did not stop. Sending SIGKILL.", camera_pid);
      kill(camera_pid, SIGKILL);
    }
    while(waitpid(camera_pid, NULL, 0) == EINTR);
    camera_pid = -1;
    log_info("Camera stopped");
  }

  if(camera_socket != -1){
    close(camera_socket);
    camera_socket = -1;
  }
}

int get_time_ms(){
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t.tv_sec*1000 + t.tv_nsec/1000000;
}
