#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

//MAP
#define SQUARE_SIZE 40//mm
#define AREA_HEIGHT 2000
#define AREA_WIDTH 3000
#define MAP_HEIGHT (AREA_HEIGHT / SQUARE_SIZE)
#define MAP_WIDTH (AREA_WIDTH / SQUARE_SIZE)
#define NB_SETS 6
#define NB_CUBES (5*NB_SETS)
#define CUBE_SIZE 58
#define CUBE_OBS (CUBE_SIZE*1.4/2.+50)
#define ROBOT_RADIUS (110 + 0)
#define CENTER_COLOR YELLOW

//HEAP
#define HEAP_SIZE (MAP_HEIGHT*MAP_WIDTH)

//Stack
#define PATTERN_SIZE 3

//ROBOT
#define ROBOT_X0 (178+70)
#define ROBOT_Y0 (438+105)
#define ROBOT_A0 0

//Cubes sets
#define CUBES_PER_SET (NB_CUBES/NB_SETS)
#define CUBE_SET(set, n) (cube[set*CUBES_PER_SET+n])

//OBSTACLES
//Construction Zone
#define CZ_W 570 //ok
#define CZ_H (180-50) //ok
#define CZ_X 400 //ok
//Water Dispenser
#define WD_R 100 //ok
#define WD_SIDE_Y 840 //ok
#define WD_BOTTOM_X 610 //ok
//Treatment Plants
#define TP_W 1200 //ok
#define TP_H 250 //ok
#define TP_X 894 //ok
#define TP_Y 2000-TP_H //ok

//Costs evaluation
#define ALIGN_COST 1.0
#define ALIGN_DIST (ROBOT_RADIUS-CUBE_SIZE)
#define APPROACH_DIST (ROBOT_RADIUS+CUBE_SIZE/2) 
#define SPEED 0.3 //m/s
#define MOVE_COEFF (1./(SPEED*1000.))
#define STACK_COST 1.0
#define ROTATE_COEFF (1./3.141592)

//Teams
typedef enum Team_E{
  GREEN_TEAM,
  ORANGE_TEAM,
}Team;

typedef enum FSM_Plan_State_E{
  FSM_PLAN_NONE,
  FSM_PLAN_T,
  FSM_PLAN_TM,
  FSM_PLAN_B,
  FSM_PLAN_BM,
  FSM_PLAN_COMPLETE
}FSM_Plan_State;

typedef enum Direction_E{
  FROM_UP,
  FROM_RIGHT,
  FROM_BOT,
  FROM_LEFT
}Direction;



//Cube colors
typedef enum Cube_Color_E{
  GREEN,
  YELLOW,
  ORANGE,
  BLACK,
  BLUE
}Cube_Color;

//Probabilities
typedef enum Probability_E{
  ZERO_PROBABILITY, //Element removed
  UNLIKELY, //Element on which we saw another robot
  UNCERTAIN, //Element on the enemy side
  LIKELY, //Element on our side
}Probability;

//Cubes
typedef struct Cube_S{
  Cube_Color color;
  Probability availability;
  uint16_t x, y;
}Cube;

typedef struct Cube_Set_S{
  uint16_t x, y;
  Probability availability;
}Cube_Set;

typedef struct Target_S{
  Cube *c;
  Direction d;
  uint16_t approach_x, approach_y;
}Target;

typedef struct Robot_S{
  uint16_t x, y;
  float angle;
  uint8_t on_target;
  Target last_target;
}Robot;

#endif

