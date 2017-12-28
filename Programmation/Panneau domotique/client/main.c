#include "main.h"

volatile sig_atomic_t stop = 0;

/*
 * Parameters :
 * IP (v4)
 * Port number
 * Note that ports under 1024 need priviledged mode, ie. root priviledge
 */
int main(int argc, char *argv[]){
  //Reading and verifying the arguments
  if(argc < 3){
    fprintf(stderr, "Too few arguments.\nclient <Server-IP> <Server-Port>\n");
    exit(EXIT_FAILURE);
  }

  //Semaphore
  sem_t *sem_score;

  //Signal
  struct sigaction act;
    
  //Shared memory
  int shm_fd;
  uint16_t *score;

  //Buffer
  char buffer[BUFFER_SIZE];
  int message_length;
  
  //Network variables
  int tcp_socket;
  struct sockaddr_in server_addr;

  /***************************/
  /*    SEM INITIALIZATION   */
  /***************************/
  sem_score = sem_open(SEM_NAME, 0);
  if(sem_score == SEM_FAILED){
    handle_error("Could not open semaphore");
  }
  
  /*****************************/
  /*   SIGNAL INITIALIZATION   */
  /*****************************/
  memset(&act, 0, sizeof(act));
  act.sa_handler = sigterm_handler;
  if(sigaction(SIGTERM, &act, NULL) == -1){
    handle_error("Could not affect SIGTERM handler");
  }
  
  /***************************/
  /*    SHM INITIALIZATION   */
  /***************************/
  //Open the shared memory
  shm_fd = shm_open(SHM_NAME, O_RDONLY, 0);
  if(shm_fd == -1){
    handle_error("Could not open shared memory");
  }

  //Map the shm to the virtual address space
  score = mmap(NULL, sizeof(*score), PROT_READ, MAP_SHARED, shm_fd, 0);
  if(score == MAP_FAILED){
    handle_error("Could not map shared memory object");
  }
  
  /***************************/
  /* NETWORK INITIALIZATIONS */
  /***************************/
  
  //Preparing sockaddr
  if(init_sockaddr(argv[1], argv[2], &server_addr) == -1){
    exit(EXIT_FAILURE);
  }

  /***************************/
  /*         LOOP            */
  /***************************/

  while(!stop){
    //Waiting for a new score to be sent
    if(sem_wait(sem_score) == -1){
      perror("WARNING : Failed to wait for semaphore");
      if(stop){
	break;
      }
    }

    //Writing score in buffer
    message_length = snprintf(buffer, BUFFER_SIZE, "%u", *score) + 1;

    printf("%s\n", buffer);
    
    //Opening a TCP socket
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socket == -1){
      perror("WARNING : Could not create socket");
    }
    
    //Opening a TCP connection
    while((connect(tcp_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) && !stop){
      perror("WARNING : The connection failed. Trying again...");
    }

    //Sending the score
    if(send(tcp_socket, buffer, message_length, MSG_NOSIGNAL) == -1){
      //If it fails, we'll try again
      perror("WARNING : Failed to write data on TCP socket.");
      sem_post(sem_score);
    }

    //A remplacer
    sleep(1);

    //Closing the TCP connection
    close(tcp_socket);
  }

  printf("Client stopped\n");
  
  //Clear things and exit
  munmap(&score, sizeof(*score));
  close(shm_fd);
  sem_close(sem_score);
  close(tcp_socket);
  
  return EXIT_SUCCESS;
}

int init_sockaddr(const char IP[], const char port[], struct sockaddr_in *addr){
  addr->sin_family = AF_INET;
  addr->sin_port = htons(atoi(port));
  if(inet_aton(IP, &addr->sin_addr) == 0){
    fprintf(stderr, "%s is not a valid IP host_address.\n", IP);
    return -1;
  }
  return 0;
}

void handle_error(const char msg[]){
  perror(msg);
  exit(EXIT_FAILURE);
  do{}while(1);
}

void sigterm_handler(int signo){
  if(signo == SIGTERM){
    stop = 1;
  }
}
