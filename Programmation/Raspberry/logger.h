#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

typedef enum Log_Type_E{
  LOG_RAW,
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO
}Log_Type;

#define LOG_MASK(log_type) (1<<log_type)

typedef enum Logger_Level_E{
  NO_LOGGING = 0,
  LOG_RAW_LEVEL = LOG_MASK(LOG_RAW),
  LOG_ERROR_LEVEL = LOG_RAW_LEVEL | LOG_MASK(LOG_ERROR),
  LOG_WARNING_LEVEL = LOG_ERROR_LEVEL | LOG_MASK(LOG_WARNING),
  LOG_INFO_LEVEL = LOG_WARNING_LEVEL | LOG_MASK(LOG_INFO)
}Logger_Level;

typedef struct Logger_S{
  //File descriptors
  int log_fd[4];
  
  //Logger level
  int level_mask;
}Logger;

void LOG_Set_Level(int level_mask);
/**
 * Set the logging level according to 'level_mask'. See Logger_Level
 * enum for predefined masks.
 */

void LOG_Set_Output(Log_Type type, int fd);
/**
 * Set the output file descriptor 'fd' for the log type 'type'.
 */


void LOG_Write(Log_Type type, const char *file, const char *function, int line, const char *fmt, ...);

void LOG_Raw(const char *fmt, ...);

#define LOG_WRITE(type, str) LOG_Write(type, __FILE__, __func__, __LINE__, str);
#define LOG_VWRITE(type, fmt, ...) LOG_Write(type, __FILE__, __func__, __LINE__, fmt, __VA_ARGS__);

#define log_error(str) LOG_WRITE(LOG_ERROR, str);
#define log_verror(fmt, ...) LOG_VWRITE(LOG_ERROR, fmt, __VA_ARGS__);

#define log_warning(str) LOG_WRITE(LOG_WARNING, str);
#define log_vwarning(fmt, ...) LOG_VWRITE(LOG_WARNING, fmt, __VA_ARGS__);

#define log_info(str) LOG_WRITE(LOG_INFO, str);
#define log_vinfo(fmt, ...) LOG_VWRITE(LOG_INFO, fmt, __VA_ARGS__);

#define log_raw(str) LOG_Raw(str);
#define log_vraw(fmt, ...) LOG_RAW(fmt, __VA_ARGS__);

#define STR_ERRNO strerror(errno)

#endif
