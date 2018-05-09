#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdint.h>
#include <stdbool.h>

#include "game_defs.h"

/**
 * Usage :
 * Call Init_Strategy once, at the beginning of the match.
 * Call Set_Construction_Plan to set the plan.
 * Call Compute_Building_Strategy
 */

typedef struct Construction_S{
  Cube* c[CUBES_PER_SET];
  uint8_t size;
  FSM_Plan_State plan_state;
}Construction;

typedef struct Building_Strategy_S{
  Target steps_tab[CUBES_PER_SET];
  Cube* materials[CUBES_PER_SET];
  int nb_materials;
  int nb_targets;
  float rank;
  float time_cost;
  int score;
}Building_Strategy;

typedef struct Builder_Context_S{
  bool on_set;
  uint8_t set_number;
  Direction align_dir;
  Construction construction;
  uint8_t nb_built;
}Builder_Context;

//Global variables
extern Team team;
extern Robot me;
extern Cube cube[NB_CUBES];
extern Cube_Set set[NB_SETS];

extern uint16_t score;
extern Building_Strategy strat;
extern Builder_Context build_ctx;
extern char color_str[5][16];

//Global defines
#define dist(x_a, y_a, x_b, y_b) sqrt(pow((y_b)-(y_a), 2) + pow((x_b)-(x_a), 2))

void Init_Construction(Construction *construction);
int Update_Construction(Cube *c, Construction *construction);
uint16_t Get_Construction_Score(Construction *construction);

//Init
void Init_Strategy();

#define Get_Cell(x, y) (map[(y==AREA_HEIGHT)?MAP_HEIGHT-1:y/SQUARE_SIZE][(x==AREA_WIDTH)?MAP_WIDTH-1:x/SQUARE_SIZE])
#define min(a, b) (((a)<(b))?(a):(b))
#define max(a, b) (((a)>=(b))?(a):(b))
#define angle(x_a, y_a, x_b, y_b) (atan2((y_b) - (y_a), (x_b) - (x_a)))

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
