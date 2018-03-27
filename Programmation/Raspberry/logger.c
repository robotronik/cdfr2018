#include "logger.h"

#define RED   "\x1B[31;1m"
#define GRN   "\x1B[32;1m"
#define YEL   "\x1B[33;1m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35;1m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37;1m"
#define RESET "\x1B[0m"


static Logger logger = {.error_fd = STDERR_FILENO,
			.warning_fd = STDERR_FILENO,
			.info_fd = STDOUT_FILENO,
			.level_mask = LOG_WARNING_LEVEL};

void LOG_Set_Level(int level_mask){
  logger.level_mask = level_mask;
}

void LOG_Set_Error_Output(int error_fd){
  logger.error_fd = error_fd;
}

void LOG_Set_Warning_Output(int warning_fd){
  logger.warning_fd = warning_fd;
}

void LOG_Set_Info_Output(int info_fd){
  logger.info_fd = info_fd;
}

void LOG_Write(Log_Type type, const char *file, const char *function, int line, const char *fmt, ...){
  if(!(logger.level_mask & type))
    return;

  va_list args;
  va_start(args, fmt);
  
  int fd;
  switch(type){
  case LOG_ERROR:
    fd = logger.error_fd;
    dprintf(fd, RED "[ERROR] ");
    break;
  case LOG_WARNING:
    fd = logger.warning_fd;
    dprintf(fd, YEL "[WARNING] ");
    break;
  case LOG_INFO:
    fd = logger.info_fd;
    dprintf(fd, GRN "[INFO] ");
    break;
  }

  dprintf(fd, "%s", file); dprintf(fd, ":");
  dprintf(fd, "%s", function); dprintf(fd, ":");
  dprintf(fd, "%d", line); dprintf(fd, "\n\t" RESET WHT);
  vdprintf(fd, fmt, args);
  dprintf(fd, "\n" RESET);

  va_end(args);
}






