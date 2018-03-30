#include "player.h"

static int player_pid = -1;
int Start_Player(const char *songs){
  if(player_pid != -1){
    Stop_Player();
  }
  
  player_pid = fork();
  if(player_pid == -1){
    log_verror("Player : fork failed : %s", STR_ERRNO);
    return -1;
  }
  
  if(player_pid == 0){
    if(signal(SIGINT, SIG_IGN) == SIG_ERR){
      log_warning("Player : failed to ignore SIGINT");
    }
    if(execl(PLAYER_PATH, PLAYER_PATH, songs, NULL) == -1){
      log_verror("Player : exec failed : %s", STR_ERRNO);
      exit(EXIT_FAILURE);
      do{}while(1);
    }
  }

  log_vinfo("Player started (pid %d)", player_pid);
  
  return 0;
}

void Stop_Player(){
  if(player_pid != -1){
    
    if(kill(player_pid, SIGTERM) == -1){
      log_vwarning("Player (pid %d) did not stop. Sending SIGKILL.", player_pid);
      kill(player_pid, SIGKILL);
    }
    while(waitpid(player_pid, NULL, 0) == EINTR);
    log_info("Player stopped");

  }
  player_pid = -1;
}
