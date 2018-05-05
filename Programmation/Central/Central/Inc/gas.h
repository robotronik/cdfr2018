#ifndef GAS_H
#define GAS_H

#include <stdint.h>

#define WAIT_TIME 50
#define ROTATE_RETRY_COUNT 5
#define ROTATION_SPEED 3.14
/*
 * When rotation fails, the robot will retry after WAIT_TIME ms, at most
 * ROTATE_RETRY_COUNT times.
 */

int Rotate(float angle);
/**
 * Try to rotate to set the angle to 'angle'. This function may fail
 * if there is an obstacle that prevents rotation.
 */

int GOGOGO(uint16_t x, uint16_t y);
/**
 * Go to the positon (x, y) (mm). Returns 0 on success, -1 on error.
 */

int Go_Straight(uint16_t x, uint16_t y);

#endif
