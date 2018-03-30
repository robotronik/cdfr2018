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
  //Start_Player(SONGS_PATH);

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

  while(!stop);
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

typedef struct PID_DATA_S{
  float Te, Kp, Ki, Kd;
}PID_DATA;

typedef enum PID_DATA_INDEX_E{
  PID_POS_SUM,
  PID_POS_DIFF,
  PID_Z
}PID_DATA_INDEX_E;

static int read_asser(FILE *f, PID_DATA *data){
  char buff[64];
  int matchs = fscanf(f, "%s\nTe %f\nKp %f\nKi %f\nKd %f\n\n", buff, &data->Te, &data->Kp, &data->Ki, &data->Kd);
  if(matchs == EOF){
    log_verror("Failed to read asser data : %s", STR_ERRNO);
  }else if(matchs != 5){
    log_error("Failed to read asser data : bad format");
  }else{
    return 0;
  }
  
  return -1;
}

static PID_DATA pid_data[3];
int load_pid_data(){
  FILE* f_asser = fopen(ASSER_PATH, "r");

  do{
    if(read_asser(f_asser, &pid_data[0])) break;
    if(read_asser(f_asser, &pid_data[1])) break;
    if(read_asser(f_asser, &pid_data[2])) break;
    fclose(f_asser);
    return 0;
  }while(0);
  
  fclose(f_asser);
  return -1;
}

void get_asser_data(RC_Server *server){
  uint8_t index;
  RC_Server_Get_Args(server, &index);
  if(index > 2){
    RC_Server_Return(server, 0, 0., 0., 0., 0.);
    return;
  }
  
  RC_Server_Return(server, 1,
		   pid_data[index].Te,
		   pid_data[index].Kp,
		   pid_data[index].Ki,
		   pid_data[index].Kd);
}
