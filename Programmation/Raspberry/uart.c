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
    //options.c_cc[VTIME] = 0;
    //options.c_cc[VMIN] = 0;

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
