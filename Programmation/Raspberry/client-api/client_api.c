#include "client_api.h"

static Scoreboard_Client sc_client = {.shm_fd = -1, .score = MAP_FAILED, .sem = SEM_FAILED, .pid = -1};
static sig_atomic_t sc_running = 0;

int SC_Start(const char *id, const char* server_ip, const char* server_port){
  if(sc_running == 1){
    fprintf(stderr, "Scoreboard_Client : Client is already running\n");
    return -1;
  }
  else{
    sc_running = 1;
  }
  
  if(server_ip == NULL || server_port == NULL){
    return -1;
  }

  /*****************/
  /* SHARED MEMORY */
  /*****************/
  //Opening shared memory
  sc_client.shm_fd = shm_open(SC_SHM_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH);
  if(sc_client.shm_fd == -1){
    perror("Scoreboard_Client : Could not open shared memory");
    SC_Stop(sc_client);
    return -1;
  }

  //Set the size of the shared memory
  if(ftruncate(sc_client.shm_fd, sizeof(*(sc_client.score))) == -1){
    perror("Scoreboard_Client : Could not set shared memory length");
    SC_Stop(sc_client);
    return -1;
  }

  //Map the shm to the virtual memory
  sc_client.score = mmap(NULL, sizeof(*(sc_client.score)), PROT_READ | PROT_WRITE, MAP_SHARED, sc_client.shm_fd, 0);
  if(sc_client.score == MAP_FAILED){
    perror("Scoreboard_Client : Failed to map shared memory");
    SC_Stop(sc_client);
    return -1;
  }
  
  /*****************/
  /*   SEMAPHORE   */
  /*****************/
  sc_client.sem = sem_open(SC_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
  if(sc_client.sem == SEM_FAILED){
    perror("Scoreboard_Client : Failed to open semaphore");
    SC_Stop(sc_client);
    return -1;
  }

  /*****************/
  /*     SIGNAL    */
  /*****************/
  //Irrelevent when the running program has more than one child
  #ifdef SC_CATCH_SIGCHLD
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = SC_SIGCHLD_Handler;
  if(sigaction(SIGCHLD, &act, NULL) == -1){
    perror("Scoreboard_Client: Failed to set SIGCHLD handler");
  }
  #endif

  /*********************/
  /*   CHILD PROCESS   */
  /*********************/
  sc_client.pid = fork();
  if(sc_client.pid == -1){
    perror("Scoreboard_Client : Failed to start child process");
    SC_Stop(sc_client);
    return -1;
  }
  else if(sc_client.pid == 0){
    if(signal(SIGINT, SIG_IGN) == SIG_ERR){
      perror("Scoreboard_Client : Failed to ignore SIGINT");
    }
    if(execl(SC_CLIENT_PATH, SC_CLIENT_PATH, id, server_ip, server_port, NULL) == -1){
      perror("Scoreboard_Client : Could not start client");
      exit(EXIT_FAILURE);
      do{}while(1);
    }
  }

  return 0;
}

void SC_Stop(){
  if(!sc_running){
    return;
  }
  
  //Child process
  if(sc_client.pid != -1){
#ifdef SC_CATCH_SIGCHLD
    struct sigaction act, old;
    memset(&act, 0, sizeof(act));
    act.sa_handler = SIG_DFL;
    sigaction(SIGCHLD, &act, &old);
#endif

    kill(sc_client.pid, SIGTERM); usleep(100);
    kill(sc_client.pid, SIGTERM); usleep(100);
    kill(sc_client.pid, SIGTERM); usleep(100);
    kill(sc_client.pid, SIGTERM); usleep(100);
    if(kill(sc_client.pid, SIGTERM) == -1){
      kill(sc_client.pid, SIGKILL);
    }fflush(stdout);
    while(waitpid(sc_client.pid, NULL, 0) == EINTR);

#ifdef SC_CATCH_SIGCHLD
    sigaction(SIGCHLD, &old, NULL);
#endif
  }
  
  //Shared memory
  if(sc_client.score != MAP_FAILED){
    munmap(sc_client.score, sizeof(*(sc_client.score)));
  }
  if(sc_client.shm_fd != -1){
    close(sc_client.shm_fd);
    shm_unlink(SC_SHM_NAME);
  }

  //Semaphore
  if(sc_client.sem != SEM_FAILED){
    sem_close(sc_client.sem);
    sem_unlink(SC_SEM_NAME);
  }

  sc_running = 0;
}

int SC_Update(uint16_t score){
  if(sc_running == 0){
    fprintf(stderr, "Scoreboard_Client : Client is not running\n");
    return -1;
  }

  *(sc_client.score) = score;
  if(sc_client.sem == NULL){
    return -1;
  }
  if(sem_post(sc_client.sem) == -1){
    perror("Scoreboard_Client : Could not post semaphore");
    return -1;
  }
  return 0;
}

#ifdef SC_CATCH_SIGCHLD
void SC_SIGCHLD_Handler(int signo){
  if(signo == SIGCHLD){
    //sc_running = 0;
    fprintf(stderr, "Scoreboard_Client : Client stopped working\n");
  }
}
#endif
