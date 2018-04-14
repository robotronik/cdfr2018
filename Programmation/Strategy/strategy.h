#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdint.h>

#include "game_defs.h"
#include "c_buff.h"

//Global variables
extern Team team;
extern Robot me;
extern Robot other_robots[3];
extern Cube cube[NB_CUBES];
extern Cube_Set set[NB_SETS];

extern uint8_t built_buildings;
extern uint16_t score;
extern Stack current_stack;

//Init
void Init_Strategy(Team team);

#define Get_Cell(x, y) (map[(y==AREA_HEIGHT)?MAP_HEIGHT-1:y/SQUARE_SIZE][(x==AREA_WIDTH)?MAP_WIDTH-1:x/SQUARE_SIZE])

//Building strategy
void Set_Construction_Plan(Cube_Color bottom, Cube_Color middle, Cube_Color top);
/**
 * This function stores the construction plan for further use in
 * strategy.
 */

int Select_Building_Materials(Stack *selected);
/**
 * This function selects cubes in the map to complete the current
 * stack. It returns 0 on success, -1 if no cubes were found.
 */

Queue* John_The_Builder(Stack *materials);
/**
 * This function apply a brute force on the selected building
 * materials to make the best decision possible. It returns a sorted
 * list of the cubes to retrieve. Please note that this list may be
 * empty : it means that either there are no more materials left,
 * either the best action to do now is to place the current building
 * on the construction zone.
 */

//Time
uint32_t Remaining_Time(void);
/**
 * Returns the remaining time, in ms.
 */

#endif
