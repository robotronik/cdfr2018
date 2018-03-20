#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <SDL2/SDL.h>
#include <sys/time.h>
#include <pthread.h>
#include "robotronik_protocol.h"
#include "remote_call.h"
#include "joystick.h"

#define SPEED B1000000

RP_Interface f4_iface;
RC_Client motor_client;
RC_Client led_client;
static int uart_fd;

uint8_t send(uint8_t *data, uint16_t len, uint32_t timeout){
  (void)timeout;

  int i;
  for(i = 0; i < len; i++){
    printf("0x%2.2X ", data[i]);
  }
  printf("\n");
  fflush(stdout);
  
  write(uart_fd, data, len);
  tcdrain(uart_fd);

  return 0;
}

void RP_Packet_Received(RP_Interface *interface, RP_Packet *packet){
#if 0
  if(interface == &f4_iface){
    printf("Packet of %d bytes received.\n", packet->len);
    int i;
    for(i = 0; i < packet->len; i++){
      printf("0x%2.2X ", packet->data[i]);
    }
    printf("\n\n");
    fflush(stdout);
  }
#endif
}

uint32_t get_tick(){
  struct timeval t;
  gettimeofday(&t, NULL);
  return (t.tv_sec * 1000) + (t.tv_usec / 1000);
}

void* receive_data(void* ptr){
  (void)ptr;
  do{
    int count = read(uart_fd, f4_iface.buffer_in, 1);
    if(count){
      RP_Process_Data(&f4_iface, f4_iface.buffer_in, count);
    }
  }while(1);
  
  return NULL;
}

int main(){
  if(SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_EVENTS) != 0){
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  //--------------------------------------------------//
  //            Joystick Initialization               //
  //--------------------------------------------------//
  SDL_JoystickEventState(SDL_ENABLE);
  
  SDL_Joystick *joystick = SDL_JoystickOpen(0);

  if(!SDL_NumJoysticks()){
    fprintf(stderr, "No joystick connected.\n");
    return EXIT_FAILURE;;
  }
  
  if(joystick){
    printf("Opened %s\n", SDL_JoystickName(joystick));
  }else{
    fprintf(stderr, "Couldn't open joystick : %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  //--------------------------------------------------//
  //            UART Initialization                   //
  //--------------------------------------------------//
  uart_fd = open("/dev/serial0", O_RDWR | O_NOCTTY);

  if(uart_fd == -1){
    perror("Open UART: ");
    return EXIT_FAILURE;
  }

  struct termios options;
  tcgetattr(uart_fd, &options);
  cfsetospeed(&options, SPEED);//Output speed
  cfsetispeed(&options, SPEED);//Input speed
  cfmakeraw(&options);
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8; //8-bits words
  options.c_cflag &= ~PARENB;//No parity
  options.c_cflag &= ~CSTOPB;//1 Stop bit
  options.c_cflag |= CLOCAL | CREAD;
  //options.c_cc[VTIME] = 0;
  //options.c_cc[VMIN] = 0;
  tcsetattr(uart_fd, TCSANOW, &options);

  tcflush(uart_fd, TCIFLUSH);

  pthread_t receive_thread;
  pthread_create(&receive_thread, NULL, receive_data, NULL);

  //--------------------------------------------------//
  //            Client Initialization                 //
  //--------------------------------------------------//
  RP_Init_Interface(&f4_iface, send, get_tick);
  RC_Client_Init(&motor_client, &f4_iface, 1);
  RC_Client_Init(&led_client, &f4_iface, 2);

  RC_Client_Add_Function(&motor_client, 0, "i", "B");
  RC_Client_Add_Function(&led_client, 0, "i", "s");

  //--------------------------------------------------//
  //                 Main Loop                        //
  //--------------------------------------------------//
  SDL_Event event;
  int run = 1;
  char str[RC_STR_SIZE];
  int pwm;
  do{
    SDL_WaitEvent(&event);
    switch(event.type){
    case SDL_JOYBUTTONDOWN:
      //event.jbutton.button
      switch(event.jbutton.button){
      case DS3_CIRCLE:
	if(RC_Call(&led_client, 0, 1, str) != 0){
	  break;
	}
	printf("%s\n", str); fflush(stdout);
	break;
      case DS3_CROSS:
	if(RC_Call(&led_client, 0, 0, str) != 0){
	  break;
	}
        printf("%s\n", str); fflush(stdout);
	break;
      default:
	break;
      }
      break;
    case SDL_JOYAXISMOTION:
      if(event.jaxis.axis == DS3_LEFT_AXIS_X){
	if(RC_Call(&motor_client, 0, event.jaxis.value, &pwm) != 0){
	  break;
	}
        printf("PWM : %d\n", pwm); fflush(stdout);
      }
      break;
    case SDL_QUIT:
      run = 0;
      break;
    default:
      break;
    }
  }while(run);
  
  close(uart_fd);

  SDL_JoystickClose(joystick);
  SDL_Quit();
  return EXIT_SUCCESS;
}
