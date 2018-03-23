#include "logger.h"


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
    dprintf(fd, "[ERROR] ");// __FILE__:__func__:__LINE__]
    break;
  case LOG_WARNING:
    fd = logger.warning_fd;
    dprintf(fd, "[WARNING] ");
    break;
  case LOG_INFO:
    fd = logger.info_fd;
    dprintf(fd, "[INFO] ");
    break;
  }

  dprintf(fd, "%s", file); dprintf(fd, ":");
  dprintf(fd, "%s", function); dprintf(fd, ":");
  dprintf(fd, "%d", line); dprintf(fd, "\n\t");
  vdprintf(fd, fmt, args);
  dprintf(fd, "\n");

  va_end(args);
}






