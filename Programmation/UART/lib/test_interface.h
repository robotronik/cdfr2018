#ifndef __TEST_INTERFACE_H
#define __TEST_INTERFACE_H

#include <stdint.h>
#include <stdio.h>
#include "robotronik_protocol.h"
#include "remote_call.h"

extern RP_Interface interface1;
extern RP_Interface interface2;
extern RC_Server server;

uint8_t send1(uint8_t *data, uint16_t size, uint32_t timeout);
uint8_t send2(uint8_t *data, uint16_t size, uint32_t timeout);
uint8_t send(uint8_t *data, uint16_t size, RP_Interface *target);

#endif
