#include "main.h"

void sigterm_handler(int signo);
void sigint_handler(int signo);

RP_Interface f4_iface;
RC_Server server;
volatile sig_atomic_t run;//uart loop
volatile sig_atomic_t stop = 0;//Ctrl+C

int main(){
  LOG_Set_Level(LOG_INFO_LEVEL);
  log_info("Initializing...");
  
  //Signals initializations
  struct sigaction act_sigterm, act_sigint;
  sigemptyset(&act_sigterm.sa_mask);
  sigemptyset(&act_sigint.sa_mask);
  memset(&act_sigterm, 0, sizeof(act_sigterm));
  memset(&act_sigint, 0, sizeof(act_sigint));
  act_sigterm.sa_handler = sigterm_handler;
  act_sigint.sa_handler = sigint_handler;
  if(sigaction(SIGTERM, &act_sigterm, NULL) == -1){
    log_error("Could not affect SIGTERM handler");
  }
  if(sigaction(SIGINT, &act_sigint, NULL) == -1){
    log_error("Could not affect SIGINT handler");
  }
  
  //UART Initialization
  int uart_fd = open_uart(UART_PATH, UART_SPEED);

  if(uart_fd == -1){
    log_error("Could not open UART");
    exit(EXIT_FAILURE);
  }

  //Interface Robotronik Protocol
  RP_Init_Interface(&f4_iface, &uart_fd, send_uart, get_tick);

  //Init server
  RC_Server_Init(&server, &f4_iface);

  if(RC_Server_Add_Function(&server, GAS, gas, "", "", RC_IMMEDIATE) == -1){
    log_error("Error while adding function");
  }
  
  if(RC_Server_Add_Function(&server, RAGEQUIT, ragequit, "", "", RC_IMMEDIATE) == -1){
    log_error("Error while adding function");
  }
  
  if(RC_Server_Add_Function(&server, WASTED, wasted, "", "", RC_IMMEDIATE) == -1){
    log_error("Error while adding function");
  }
  
  if(RC_Server_Add_Function(&server, SO_POINTS_MUCH_SCORE, so_points_much_score, "B", "", RC_IMMEDIATE) == -1){
    log_error("Error while adding function");
  }
  
  if(RC_Server_Add_Function(&server, READ_THIS_DAMN_PLAN_MORRIS, read_this_damn_plan_morris, "", "bs", RC_IMMEDIATE) == -1){
    log_error("Error while adding function");
  }
  
  if(RC_Server_Add_Function(&server, RANDOM_STUFF, random_stuff, "s", "", RC_IMMEDIATE) == -1){
    log_error("Error while adding function");
  }

  /*
  if(RC_Server_Add_Function(&server, GET_ASSER_DATA, get_asser_data, "b", "bfffii", RC_IMMEDIATE) == -1){
    log_error("Error while adding function");
  }

  if(load_pid_data()){
    log_error("Failed to load asser data");
    }*/

  //Mode boombox
  Start_Player(SONGS_PATH);

  //Receive data
  run = 1;
  do{
    int count = read(uart_fd, f4_iface.buffer_in, 1);
    if(count && count != -1){
      RP_Process_Data(&f4_iface, f4_iface.buffer_in, count);
    }
  }while(run && !stop);

  if(stop){
    //Stop the nucleo
    f4_iface.s_packet.len = 1;
    f4_iface.s_packet.data[0] = 0xFF;
    RP_Sync(&f4_iface, RC_TRANSFERT_TIMEOUT);
    RP_Send(&f4_iface, &f4_iface.s_packet, RC_TRANSFERT_TIMEOUT);
  }
  
  close(uart_fd);

  log_info("Main loop stopped. Idle state.");

  blink_end();

  log_info("Quit");
  
  return EXIT_SUCCESS;
}

void sigterm_handler(int signo){
  if(signo == SIGTERM){
    printf("\n");
    log_warning("SIGTERM received.");
    SC_Stop();
    Stop_Camera();
    Stop_Player();
    stop = 1;
  }
}

void sigint_handler(int signo){
  if(signo == SIGINT){
    printf("\n");
    log_warning("SIGINT received.");
    SC_Stop();
    Stop_Camera();
    Stop_Player();
    stop = 1;
  }
}

void blink_end(){
  wiringPiSetup();
  softPwmCreate(PWM_PIN, 0, 255);

  const int T = 2;
  while(!stop){
    int i;
    for(i = 0; i < 20*T; i++){
      softPwmWrite(PWM_PIN, (int) ((1.+cos(2.*3.141592*((double)i)/(T*20.-1.)))*255.*0.5));
      usleep(50000);
    }
  }
  usleep(50000);
  softPwmWrite(PWM_PIN, 0);
  usleep(50000);
}
