#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#include "main.h"

int Start_Camera();
void Stop_Camera();
int get_time_ms();

#endif
