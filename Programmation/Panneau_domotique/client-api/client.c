#include "client.h"

volatile sig_atomic_t stop = 0;

#if LOG_INFO == 1
#define INFO(info) {printf("INFO: "); printf(info); printf("\n"); fflush(stdout);}
#define VINFO(info, ...) {printf("INFO: "); printf(info, __VA_ARGS__); printf("\n"); fflush(stdout);}
#else
#define INFO(info)
#define VINFO(info, ...)
#endif

/*
 * Parameters :
 * ID
 * IP (v4)
 * Port number
 * Note that ports under 1024 need priviledged mode, ie. root priviledge
 */
int main(int argc, char *argv[]){
  //Reading and verifying the arguments
  if(argc < 4){
    fprintf(stderr, "Too few arguments.\nclient <ID> <Server-IP> <Server-Port>\n");
    exit(EXIT_FAILURE);
  }
  INFO("Client started");
  
  //Semaphore
  sem_t *sem_score;
  struct timespec time;

  //Signal
  struct sigaction act_term, act_int;
    
  //Shared memory
  int shm_fd;
  uint16_t *score, score_tmp;

  //Buffer
  char buffer[BUFFER_SIZE];
  int id;
  int message_length;
  
  //Network variables
  int tcp_socket;
  struct sockaddr_in server_addr;

  //ID
  id = atoi(argv[1]);
  if(id != 1 && id != 2){
    fprintf(stderr, "Bad ID : must be 1 or 2\n");
    exit(EXIT_FAILURE);
  }
  
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
  memset(&act_term, 0, sizeof(act_term));
  memset(&act_int, 0, sizeof(act_int));
  act_term.sa_handler = sigterm_handler;
  act_int.sa_handler = sigint_handler;
  if(sigaction(SIGTERM, &act_term, NULL) == -1){
    handle_error("Could not affect SIGTERM handler");
  }
  if(sigaction(SIGINT, &act_term, NULL) == -1){
    handle_error("Could not affect SIGINT handler");
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
  if(init_sockaddr(argv[2], argv[3], &server_addr) == -1){
    exit(EXIT_FAILURE);
  }

  /***************************/
  /*         LOOP            */
  /***************************/
  INFO("Client initialization successful");
  while(!stop){
    //Waiting for a new score to be sent
    if(clock_gettime(CLOCK_REALTIME, &time) != 0){
      perror("WARNING : Couldn't get current time\n");

      //Fallback on sleep mode
      int r = sem_trywait(sem_score);
      if(r == 0){
	score_tmp = *score;
	INFO("Unlocked for update");
      }else if(errno == EAGAIN){
	INFO("Sending the score again in one second...");
	sleep(1);
      }else{
	perror("WARNING : Failed to wait for semaphore");
      }
    }
    
    time.tv_sec += 1;
    int r = sem_timedwait(sem_score, &time);
    if(r == 0){
      INFO("Unlocked for update");
      score_tmp = *score;
    }else if(errno != ETIMEDOUT){
      perror("WARNING : Failed to wait for semaphore");
      if(stop){
	break;
      }
    }else{
      INFO("Unlocked after timeout");
    }
    
    //Writing score in buffer
    message_length = snprintf(buffer, BUFFER_SIZE, "0%d%u", id, score_tmp) + 1;
    /*
     * Message :
     * '0' | 'ID' | "score" | '\0'
     */

    VINFO("Message to send: '%s' (id = %d, score = %u)", buffer, id, score_tmp);
    
    //Opening a TCP socket
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socket == -1){
      perror("WARNING : Could not create socket");
    }
    
    //Opening a TCP connection
    VINFO("Connecting to %s:%d...", argv[2], atoi(argv[3]));
    while((connect(tcp_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) && !stop){
      perror("WARNING : Connection failed. Trying again...");
    }

    //Sending the score
    INFO("Connected. Sending score...");
    if(send(tcp_socket, buffer, message_length, MSG_NOSIGNAL) == -1){
      //If it fails, we'll try again
      perror("WARNING : Failed to write data on TCP socket");
      sem_post(sem_score);
    }else{
      INFO("Message sent");
    }

    //Closing the TCP connection
    close(tcp_socket);
    INFO("Disconnected\n");
  }

  INFO("Client stopped\n");
  
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

void sigint_handler(int signo){
  if(signo == SIGINT){
    stop = 1;
  }
}
