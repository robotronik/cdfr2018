#ifndef GAS_H
#define GAS_H

#include <stdint.h>

int GOGOGO(uint16_t x, uint16_t y);
/**
 * Go to the positon (x, y) (mm). Returns 0 on success, -1 on error.
 */

int Go_Straight(uint16_t x, uint16_t y);

#endif
