#ifndef GAS_H
#define GAS_H

#include <stdint.h>
#include <stdbool.h>

#define WAIT_TIME 50
#define ROTATE_RETRY_COUNT 20
#define ROTATION_SPEED 3.14
/*
 * When rotation fails, the robot will retry after WAIT_TIME ms, at most
 * ROTATE_RETRY_COUNT times.
 */
#define MOVE_RETRY_COUNT 50
#define MAX_SPEED 200.
#define MIN_SPEED 10.

int Rotate(float angle);
/**
 * Try to rotate to set the angle to 'angle'. This function may fail
 * if there is an obstacle that prevents rotation.
 */

int GOGOGO(uint16_t x, uint16_t y);
/**
 * Go to the positon (x, y) (mm). Returns 0 on success, -1 on error.
 */

int Go_Straight(uint16_t x, uint16_t y, bool forward, float speed_ratio);

#endif