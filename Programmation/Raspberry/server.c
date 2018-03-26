#include "server.h"

void gas(RC_Server *server){
  RC_Server_Return(server);
  
  log_info("Received start request");
  
  //Start scoreboard server
  if(SC_Start(ID_ROBOT, IP_ESP, PORT_ESP) == -1){
    log_verror("Could not start Scoreboard Client as %d at %s:%s", ID_ROBOT, IP_ESP, PORT_ESP);
  }else{
    log_info("Scoreboard Client started");
  }

  //Start match song
  Stop_Player();
  Start_Player(MATCH_SONG);

  //Start recording
  Start_Camera();
}

void ragequit(RC_Server *server){
  RC_Server_Return(server);
  log_info("Received stop request");
  
  run = 0;
  SC_Stop();

  //Restart boombox
  Stop_Player();
  Start_Player(SONGS_PATH);
  
}

void wasted(RC_Server *server){
  RC_Server_Return(server);
  log_info("Received error");
  
  //Start error song
  Stop_Player();
  Start_Player(ERROR_SONG);
  
  run = 0;
  SC_Stop();
}

void so_points_much_score(RC_Server *server){    
  uint16_t score;
  RC_Server_Get_Args(server, &score);
  RC_Server_Return(server);
  
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
}

void read_this_damn_plan_morris(RC_Server *server){
  char colors[4];

  int r = Read_Plan(colors);
  
  RC_Server_Return(server, r, colors);
}

void random_stuff(RC_Server *server){
  char str[RC_STR_SIZE];
  RC_Server_Get_Args(server, str);
  RC_Server_Return(server);

  log_vinfo("Logs received : %s", str);
  printf("%s\n", str);
}
