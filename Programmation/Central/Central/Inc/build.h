#ifndef BUILD_H
#define BUILD_H

#include <stdbool.h>
#include <stdint.h>
#include "game_defs.h"

#define DIST_ALIGN (CUBE_OBS + ROBOT_RADIUS)
#define DIST_APPROACH 103 //ok
#define APPROACH_SPEED_RATIO 0.1

void Build_Building();

int Get_Cube(Target *t);

void Place_Building();

#endif
