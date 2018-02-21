#ifndef __TEST_INTERFACE_H
#define __TEST_INTERFACE_H

#include <stdint.h>
#include <stdio.h>

uint8_t send(uint8_t *data, uint16_t size, uint32_t timeout);

uint8_t receive(uint8_t *data, uint16_t size, uint32_t timeout);

#endif
