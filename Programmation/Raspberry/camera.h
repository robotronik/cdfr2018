#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#include "main.h"

#define CV_TIMEOUT 3000

typedef enum CV_State_E{
  CV_RUNNING,
  CV_ERR_TIMEOUT,
  CV_ERR,
  CV_OK
}CV_State;

int Start_Camera();
void Stop_Camera();
CV_State Read_Plan(char colors[4]);
int get_time_ms();

#endif
