#include "main.h"

#include <stdlib.h>
#include <stdbool.h>

#include "uart/robotronik_protocol.h"
#include "uart/remote_call.h"
#include "logger.h"



RP_Interface f4_iface;
RC_Server server;
volatile bool run;

void RP_Packet_Received(RP_Interface *interface, RP_Packet *packet){
  if(interface == &f4_iface){
#ifdef DEBUG_UART
    printf("Packet of %d bytes received.\n", packet->len);
    int i;
    for(i = 0; i < packet->len; i++){
      printf("0x%2.2X ", packet->data[i]);
    }
    printf("\n\n");
    fflush(stdout);
#endif
    
    RC_Server_Get_Request(&server, packet);
  }
}


int main(){
  log_info("Initializing...");

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
  
  //Receive data
  run = 1;
  do{
    int count = read(uart_fd, f4_iface.buffer_in, 1);
    if(count && count != -1){
      RP_Process_Data(&f4_iface, f4_iface.buffer_in, count);
    }
  }while(run);
  
  close(uart_fd);

  return EXIT_SUCCESS;
}
