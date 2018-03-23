#include "logger.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <SDL2/SDL.h>
#include <sys/time.h>
#include <pthread.h>
#include "uart/robotronik_protocol.h"
#include "uart/remote_call.h"
#include "main.h"

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
  //--------------------------------------------------//
  //            UART Initialization                   //
  //--------------------------------------------------//
  log_info("Initializing UART...");
  uart_fd = open_uart(UART_PATH, UART_SPEED);

  if(uart_fd == -1){
    log_error("Could not open UART");
    exit(EXIT_FAILURE);
  }

  close(uart_fd);
  
  #if 0
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

  close(uart_fd);

  #endif
  return EXIT_SUCCESS;
}
