#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "pi_client.h"

#define Get_Ticks() HAL_GetTick()
#define Print(fmt, ...) (PI_Log(fmt, __VA_ARGS__))

#endif
