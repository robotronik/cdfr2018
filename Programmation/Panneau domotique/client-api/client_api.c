#include "client_api.h"

Scoreboard_Client sc_client;
sig_atomic_t sc_running = 0;

uint16_t* SC_Start(const char* server_ip, const char* server_port){
  if(sc_running == 1){
    fprintf(stderr, "Scoreboard_Client : Client is already running\n");
    return NULL;
  }
  else{
    sc_running = 1;
  }
  
  if(server_ip == NULL || server_port == NULL){
    return NULL;
  }

  /*****************/
  /* SHARED MEMORY */
  /*****************/
  //Opening shared memory
  sc_client.shm_fd = shm_open(SC_SHM_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH);
  if(sc_client.shm_fd == -1){
    perror("Scoreboard_Client : Could not open shared memory");
    SC_Stop(sc_client);
    return NULL;
  }

  //Set the size of the shared memory
  if(ftruncate(sc_client.shm_fd, sizeof(*(sc_client.score))) == -1){
    perror("Scoreboard_Client : Could not set shared memory length");
    SC_Stop(sc_client);
    return NULL;
  }

  //Map the shm to the virtual memory
  sc_client.score = mmap(NULL, sizeof(*(sc_client.score)), PROT_READ | PROT_WRITE, MAP_SHARED, sc_client.shm_fd, 0);
  if(sc_client.score == MAP_FAILED){
    perror("Scoreboard_Client : Failed to map shared memory");
    SC_Stop(sc_client);
    return NULL;
  }
  
  /*****************/
  /*   SEMAPHORE   */
  /*****************/
  sc_client.sem = sem_open(SC_SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
  if(sc_client.sem == SEM_FAILED){
    perror("Scoreboard_Client : Failed to open semaphore");
    SC_Stop(sc_client);
    return NULL;
  }

  /*****************/
  /*     SIGNAL    */
  /*****************/
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = SC_SIGCHLD_Handler;
  if(sigaction(SIGCHLD, &act, NULL) == -1){
    perror("Scoreboard_Client: Failed to set SIGCHLD handler");
  }

  /*********************/
  /*   CHILD PROCESS   */
  /*********************/
  sc_client.pid = fork();
  if(sc_client.pid == -1){
    perror("Scoreboard_Client : Failed to start child process");
    SC_Stop(sc_client);
    return NULL;
  }
  else if(sc_client.pid == 0){
    if(execl(SC_CLIENT_PATH, SC_CLIENT_PATH, server_ip, server_port, NULL) == -1){
      perror("Scoreboard_Client : Could not start client");
      exit(EXIT_FAILURE);
      do{}while(1);
    }
  }

  return sc_client.score;
}

void SC_Stop(){
  //Child process
  if(sc_client.pid != -1){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = SIG_DFL;
    sigaction(SIGCHLD, &act, NULL);
    if(kill(sc_client.pid, SIGTERM) == -1){
      kill(sc_client.pid, SIGKILL);
    }
    while(waitpid(sc_client.pid, NULL, 0) == EINTR);
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
    sem_unlink(SC_SEM_NAME);
  }

  sc_running = 0;
}

int SC_Update(){
  if(sc_running == 0){
    fprintf(stderr, "Scoreboard_Client : Client is not running\n");
    return -1;
  }
  
  if(sc_client.sem == NULL){
    return -1;
  }
  if(sem_post(sc_client.sem) == -1){
    perror("Scoreboard_Client : Could not post semaphore");
    return -1;
  }
  return 0;
}

void SC_SIGCHLD_Handler(int signo){
  if(signo == SIGCHLD){
    sc_running = 0;
    fprintf(stderr, "Scoreboard_Client : Client stopped working\n");
  }
}