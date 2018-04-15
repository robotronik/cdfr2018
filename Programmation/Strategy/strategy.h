#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdint.h>

#include "game_defs.h"

typedef struct Construction_S{
  Cube* c[CUBES_PER_SET];
  uint8_t size;
  FSM_Plan_State plan_state;
}Construction;

typedef struct Building_Strategy_S{
  Target steps_tab[CUBES_PER_SET];
  Cube* materials[CUBES_PER_SET];
  int nb_materials;
  float rank;
  float time_cost;
  int score;
}Building_Strategy;

//Global variables
extern Team team;
extern Robot me;
extern Other_Robot other_robots[3];
extern Cube cube[NB_CUBES];
extern Cube_Set set[NB_SETS];

extern uint8_t built_buildings;
extern uint16_t score;
extern Construction current_construction;
extern Building_Strategy strat;

void Init_Construction(Construction *construction);
int Update_Construction(Cube *c, Construction *construction);

//Init
void Init_Strategy(Team team);

#define Get_Cell(x, y) (map[(y==AREA_HEIGHT)?MAP_HEIGHT-1:y/SQUARE_SIZE][(x==AREA_WIDTH)?MAP_WIDTH-1:x/SQUARE_SIZE])


//Building strategy
void Set_Construction_Plan(Cube_Color bottom, Cube_Color middle, Cube_Color top);
/**
 * This function stores the construction plan for further use in
 * strategy.
 */

int Select_Building_Materials(Cube* materials[CUBES_PER_SET]);
/**
 * This function selects cubes in the map to complete the current
 * stack. It returns the number of found cubes.
 */

void John_The_Builder();
/**
 * This function apply a brute force on the selected building
 * materials to make the best decision possible. It returns a sorted
 * list of the cubes to retrieve. Please note that this list may be
 * empty : it means that either there are no more materials left,
 * either the best action to do now is to place the current building
 * on the construction zone.
 */

void Compute_Building_Strategy(void);

//Time
uint32_t Remaining_Time(void);
/**
 * Returns the remaining time, in ms.
 */

#endif
