#include "test.h"

void sigterm_handler(int signo){
  if(signo == SIGTERM){
    printf("SIGTERM received.\n");
    SC_Stop();
    exit(EXIT_SUCCESS);
    do{}while(1);
  }
}

void sigint_handler(int signo){
  if(signo == SIGINT){
    printf("SIGINT received.\n");
    SC_Stop();
    exit(EXIT_SUCCESS);
    do{}while(1);
  }
}

void handle_error(const char msg[]){
  perror(msg);
  exit(EXIT_FAILURE);
  do{}while(1);
}

int main(){
  //Signal
  struct sigaction act_sigterm, act_sigint;
  memset(&act_sigterm, 0, sizeof(act_sigterm));
  memset(&act_sigint, 0, sizeof(act_sigint));
  act_sigterm.sa_handler = sigterm_handler;
  act_sigint.sa_handler = sigint_handler;
  if(sigaction(SIGTERM, &act_sigterm, NULL) == -1){
    handle_error("Could not affect SIGTERM handler");
  }
  if(sigaction(SIGINT, &act_sigint, NULL) == -1){
    handle_error("Could not affect SIGINT handler");
  }
  
  SC_Start("2", "192.168.0.4", "80");
  uint16_t score;
  int i;
  for(i=0; i < 100; i++){
    score = i;
    SC_Update(score);
    sleep(1);
  }
  sleep(120);
  SC_Stop();
  return EXIT_SUCCESS;
}

