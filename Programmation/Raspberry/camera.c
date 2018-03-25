#include "camera.h"

static int camera_pid = -1;
static int camera_socket = -1;

int Start_Camera(){
  if(camera_pid != -1){
    Stop_Camera();
  }

  //Socket
  camera_socket = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
  if(camera_socket == -1){
    log_verror("Camera : failed to create socket : %s", STR_ERRNO);
    return -1;
  }

  //unlink camera_socket
  int unlink_pid = fork();
  if(!unlink_pid){
    execlp("unlink", "unlink", CAMERA_SOCKET, NULL);
  }else if(unlink_pid != -1){
    waitpid(unlink_pid, NULL, 0);
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

void Stop_Camera(){
  if(camera_pid != -1){
    if(kill(camera_pid, SIGTERM) == -1){
      log_vwarning("Player (pid %d) did not stop. Sending SIGKILL.", camera_pid);
      kill(camera_pid, SIGKILL);
    }
    while(waitpid(camera_pid, NULL, 0) == EINTR);
    log_info("Camera stopped");
  }

  if(camera_socket != -1){
    close(camera_socket);
  }
  
  camera_pid = -1;
}

int get_time_ms(){
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t.tv_sec*1000 + t.tv_nsec/1000000;
}