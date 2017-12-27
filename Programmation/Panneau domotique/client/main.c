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
  if(argc < 5){
    fprintf(stderr, "Too few arguments.\nclient <Host-IP> <Host-Port> <Server-IP> <Server-Port>\n");
    exit(EXIT_FAILURE);
  }

  //Semaphore
  sem_t *score_sem;

  //Signal
  struct sigaction act;
    
  //Shared memory
  int shm_fd;
  uint16_t *score;
  
  //Network variables
  int tcp_socket;
  struct sockaddr_in host_addr, server_addr;

  /***************************/
  /*    SEM INITIALIZATION   */
  /***************************/
  score_sem = sem_open(SEM_NAME, 0);
  if(score_sem == SEM_FAILED){
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
  if(init_sockaddr(argv[1], argv[2], &host_addr) == -1){
    exit(EXIT_FAILURE);
  }

  if(init_sockaddr(argv[3], argv[4], &server_addr) == -1){
    exit(EXIT_FAILURE);
  }

  //Opening a TCP socket
  tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(tcp_socket == -1){
    handle_error("Could not create socket");
  }

  //Binding the socket to the given IP host_address and port
  if(bind(tcp_socket, (struct sockaddr*) &host_addr, sizeof(host_addr)) == -1){
    handle_error("Could not bind socket");
  }

  /***************************/
  /*         LOOP            */
  /***************************/

  while(!stop){
    if(connect(tcp_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
      handle_error("Could not connect");
    }
    
    char buffer[] = "127";
    write(tcp_socket, buffer, 4);
    
    buffer[2] = '5';
    write(tcp_socket, buffer, 4);
  }
  


  //Clear things and exit
  munmap(&score, sizeof(*score));
  close(shm_fd);
  sem_close(score_sem);
  
  return EXIT_SUCCESS;
}

int init_sockaddr(const char IP[], const char port[], struct sockaddr_in *addr){
  addr->sin_family = AF_INET;
  addr->sin_port = htons(atoi(port));
  if(inet_aton(IP, &addr->sin_addr) == 0){
    fprintf(stderr, "%s is not a valid IP host_address.", IP);
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
