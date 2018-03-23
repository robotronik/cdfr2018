#include "server.h"

void gas(RC_Server *server){
  log_info("Received start request");
  
  //Start scoreboard server
  if(SC_Start(ID_ROBOT, IP_ESP, PORT_ESP) == -1){
    log_verror("Could not start Scoreboard Client as %2 at %s:%s", ID_ROBOT, IP_ESP, PORT_ESP);
  }else{
    log_info("Scoreboard Client started");
  }
  
  RC_Server_Return(server);
}

void ragequit(RC_Server *server){
  log_info("Received stop request");
  
  run = 0;
  SC_Stop();
  
  RC_Server_Return(server);
}

void wasted(RC_Server *server){
 
  run = 0;
  SC_Stop();
  RC_Server_Return(server);
}

void so_points_much_score(RC_Server *server){    
  uint16_t score;
  RC_Server_Get_Args(server, &score);

  log_vinfo("Received new score : %d", score);
  
  if(SC_Update(score) == -1){
    //Just in case, but should never happen
    log_warning("Failed to update score. Trying to restart client...");
    SC_Stop();
    SC_Start(ID_ROBOT, IP_ESP, PORT_ESP);
    if(SC_Update(score) == -1){
      log_error("Attempt to recover Scoreboard Client failed.");
    }
  }
  
  RC_Server_Return(server);
}

void read_this_damn_plan_morris(RC_Server *server){

  
}

void random_stuff(RC_Server *server){
  log_info("Logs received");
  char str[RC_STR_SIZE];
  RC_Server_Get_Args(server, str);

  printf("%s", str);
  
  RC_Server_Return(server);
}
