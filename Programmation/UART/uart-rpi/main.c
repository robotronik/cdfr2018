#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "rpv1.h"

#define SPEED B1000000

static RP_Interface iface;
static int uart_fd;

uint8_t send(uint8_t *data, uint16_t len, uint32_t timeout){
  write(uart_fd, data, len);
  tcdrain(uart_fd);

  return 0;
}

void RP_Packet_Received(RP_Interface *interface, RP_Packet *packet){
  if(interface == &iface){
    printf("Packet of %d bytes received.\n", packet->len);
    int i;
    for(i = 0; i < packet->len; i++){
      printf("0x%2.2X ", packet->data[i]);
    }
    printf("\n\n");
    fflush(stdout);
  }
}

int main(){
  RP_Init_Interface(&iface, send);

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
  tcsetattr(uart_fd, TCSANOW, &options);

  tcflush(uart_fd, TCIFLUSH);

#if 1
  RP_Packet packet;
  packet.len = 2;
  
  packet.data[0] = 0;
  packet.data[1] = 128;

  RP_Send(&iface, &packet, 1);
#endif
  
  //Temporary

  while(1){
    int count = read(uart_fd, iface.buffer_in, 1);
    if(count == -1){
      perror("Read: ");
    }
    else{
      RP_Process_Data(&iface, iface.buffer_in, count);
    }
  }
  
  close(uart_fd);
  return EXIT_SUCCESS;
}
