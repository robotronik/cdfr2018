#ifndef __MAIN_H_
#define __MAIN_H_

#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "uart/robotronik_protocol.h"
#include "uart/remote_call.h"
#include "uart.h"
#include "logger.h"
#include "server.h"

#define UART_PATH "/dev/serial0"
#define UART_SPEED B1000000

#define ID_ROBOT "1"
#define IP_ESP "192.168.0.4"
#define PORT_ESP "80"

extern RP_Interface f4_iface;
extern RC_Server server;
extern volatile sig_atomic_t run;

#endif
