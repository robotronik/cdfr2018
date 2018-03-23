#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

typedef enum Log_Type_E{
  LOG_ERROR = 1, LOG_WARNING = 2, LOG_INFO = 4
}Log_Type;

typedef enum Logger_Level_E{
  NO_LOGGING = 0,
  LOG_ERROR_LEVEL = LOG_ERROR,
  LOG_WARNING_LEVEL = LOG_ERROR | LOG_WARNING,
  LOG_INFO_LEVEL = LOG_ERROR | LOG_WARNING | LOG_INFO
}Logger_Level;

typedef struct Logger_S{
  //File descriptors
  int error_fd;
  int warning_fd;
  int info_fd;
  
  //Logger level
  int level_mask;
}Logger;

void LOG_Set_Level(int level_mask);
void LOG_Set_Error_Output(int error_fd);
void LOG_Set_Warning_Output(int warning_fd);
void LOG_Set_Info_Output(int info_fd);

void LOG_Write(Log_Type type, const char *file, const char *function, int line, const char *fmt, ...);

#define LOG_WRITE(type, str) LOG_Write(type, __FILE__, __func__, __LINE__, str);
#define LOG_VWRITE(type, fmt, ...) LOG_Write(type, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__);

#define log_error(str) LOG_WRITE(LOG_ERROR, str);
#define log_verror(fmt, ...) LOG_VWRITE(LOG_ERROR, fmt, __VA_ARGS__);

#define log_warning(str) LOG_WRITE(LOG_WARNING, str);
#define log_vwarning(fmt, ...) LOG_VWRITE(LOG_WARNING, fmt, __VA_ARGS__);

#define log_info(str) LOG_WRITE(LOG_INFO, str);
#define log_vinfo(fmt, ...) LOG_VWRITE(LOG_INFO, fmt, __VA_ARGS__);

#define STR_ERRNO strerror(errno)

#endif
