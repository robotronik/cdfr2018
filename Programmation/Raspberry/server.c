#include "server.h"

void gas(RC_Server *server){
  RC_Server_Return(server);
  log_info("Received start request");
  
  //Start scoreboard client
  if(SC_Start(ID_ROBOT, IP_ESP, PORT_ESP) == -1){
    log_verror("Could not start Scoreboard Client as %d at %s:%s", ID_ROBOT, IP_ESP, PORT_ESP);
  }else{
    log_info("Scoreboard Client started");
  }

  //GAS GAS GAS
  Stop_Player();
  Start_Player(MATCH_SONG);

  //Start recording
  Start_Camera();
}

void ragequit(RC_Server *server){
  RC_Server_Return(server);
  log_info("Received stop request");
  
  //Stop scoreboard client
  SC_Stop();

  //Restart boombox
  Stop_Player();
  Start_Player(SONGS_PATH);

  //Stop recording
  Stop_Camera();

  //Stop UART
  run = 0;
}  

void wasted(RC_Server *server){
  RC_Server_Return(server);
  log_info("Received error");

  //Stop scoreboard client
  SC_Stop();
  
  //Start error song
  Stop_Player();
  Start_Player(ERROR_SONG);

  //Stop recording
  Stop_Camera();

  //Stop uart
  run = 0;
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

  //Protection against unterminated strings
  str[RC_STR_SIZE-1] = '\0';
  
  log_raw(str);
}
