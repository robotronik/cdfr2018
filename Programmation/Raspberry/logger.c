#include "logger.h"

#define RED   "\x1B[31;1m"
#define GRN   "\x1B[32;1m"
#define YEL   "\x1B[33;1m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35;1m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37;1m"
#define RESET "\x1B[0m"

static Logger logger = {.log_fd = {[LOG_RAW] = STDOUT_FILENO,
				   [LOG_ERROR] = STDERR_FILENO,
				   [LOG_WARNING] = STDERR_FILENO,
				   [LOG_INFO] = STDOUT_FILENO},
			.level_mask = LOG_WARNING_LEVEL};

void LOG_Set_Level(int level_mask){
  logger.level_mask = level_mask;
}

void LOG_Set_Output(Log_Type type, int fd){
  logger.log_fd[type] = fd;
}

void LOG_Write(Log_Type type, const char *file, const char *function, int line, const char *fmt, ...){
  static char str_type[4][32] = {[LOG_RAW] = MAG "[RAW]",
				 [LOG_ERROR] = RED "[ERROR]",
				 [LOG_WARNING] = YEL "[WARNING]",
				 [LOG_INFO] = GRN "[INFO]"};
  
  if(!(logger.level_mask & LOG_MASK(type)))
    return;
  
  va_list args;
  va_start(args, fmt);

  int fd = logger.log_fd[type];
  
  dprintf(fd, "%s %s:%s:%d\n\t" RESET WHT, str_type[type], file, function, line);
  vdprintf(fd, fmt, args);
  dprintf(fd, "\n" RESET);

  va_end(args);
}

void LOG_Raw(const char *fmt, ...){
  va_list args;
  va_start(args, fmt);

  vdprintf(logger.log_fd[LOG_RAW], fmt, args);
  
  va_end(args);
}


