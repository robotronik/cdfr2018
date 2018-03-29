#include "uart.h"

int open_uart(const char* path, speed_t speed){
  //Open the UART file
  int fd = open(path,
		//Close on exec, to avoid childs to access UART
		O_CLOEXEC
		//Allow reading and writing
		| O_RDWR
		//Don't use the file as a terminal to control the program
		| O_NOCTTY);
  if(fd == -1){
    log_verror("Could not open %s : %s", path, STR_ERRNO);
    return -1;
  }

  //Configure UART
  do{
    struct termios options;
    //Get current options
    if(tcgetattr(fd, &options)){
      log_vwarning("Could not get uart options on %s : %s", path, STR_ERRNO);
    }
    
    cfsetospeed(&options, speed);//Output speed
    cfsetispeed(&options, speed);//Input speed
    cfmakeraw(&options);//Use tty as a raw terminal (no control)
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8; //8-bits words
    options.c_cflag &= ~PARENB;//No parity
    options.c_cflag &= ~CSTOPB;//1 Stop bit
    options.c_cflag |= CLOCAL | CREAD;
    options.c_cc[VTIME] = 1;//100ms timeout
    options.c_cc[VMIN] = 1;//Read blocks until one byte is received

    //Set options
    if(tcsetattr(fd, TCSANOW, &options)){
      //Not a reliable check, see man termios
      log_verror("Could not set uart options on %s : %s", path, STR_ERRNO);
      break;
    }

    //Flush any old input data
    tcflush(fd, TCIFLUSH);
  
    return fd;
    
  }while(0);
  
  close(fd);
  return -1;
}

uint8_t send_uart(void* link_handler, uint8_t *data, uint16_t len, uint32_t timeout){
  (void)timeout;

#ifdef DEBUG_UART
  int i;
  for(i = 0; i < len; i++){
    printf("0x%2.2X ", data[i]);
  }
  printf("\n");
  fflush(stdout);
#endif
  
 write(*((int*) link_handler), data, len);
 tcdrain(*((int*) link_handler));

  return 0;
}

uint32_t get_tick(){
  struct timeval t;
  gettimeofday(&t, NULL);
  return (t.tv_sec * 1000) + (t.tv_usec / 1000);
}

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

void RP_Error_Handler(RP_Interface *iface, uint16_t error){
  (void)iface;
  switch(RP_ERROR_TYPE(error)){
  case RP_ERR_INTERNAL:
    //printf("Internal error: ");
    switch(RP_INTERNAL_ERROR(error)){
    case RP_ERR_ILLEGAL_ARGUMENTS:
      log_warning("Illegal arguments");
      break;
    case RP_ERR_BUFFER_OVERFLOW:
      log_warning("Buffer overflow");
      break;
    default:
      log_warning("Unknown");
      break;
    }
    break;
  case RP_ERR_LINK:
    //printf("Link error: ");
    switch(RP_LINK_ERROR(error)){
    case RP_ERR_TIMEOUT:
      log_warning("Timeout");
      break;
    case RP_ERR_UNEXPECTED_EOF:
      log_warning("Unexpected EOF. Synchronized.");
      break;
    case RP_ERR_SIZE:
      log_warning("Size error");
      break;
    case RP_ERR_CRC:
      log_warning("CRC Error");
      break;
    default:
      log_warning("Unknown");
      break;
    }
    break;
  default:
    //printf("Unknown error\n");
    break;
  }
}
