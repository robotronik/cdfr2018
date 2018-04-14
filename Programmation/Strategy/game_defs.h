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
#define ROBOT_RADIUS (110 + 0)
#define CENTER_COLOR YELLOW

//HEAP
#define HEAP_SIZE (MAP_HEIGHT*MAP_WIDTH)

//Stack
#define PATTERN_SIZE 3

//ROBOT
#define ROBOT_X0 200
#define ROBOT_Y0 450

//Cubes sets
#define CUBES_PER_SET (NB_CUBES/NB_SETS)
#define CUBE_SET(set, n) (cube[set*CUBES_PER_SET+n])

//OBSTACLES
//Construction Zone
#define CZ_W 540 
#define CZ_H 150
#define CZ_X 400
//Water Dispenser
#define WD_R 50 
#define WD_SIDE_Y 840
#define WD_BOTTOM_X 610
//Treatment Plants
#define TP_W 1200
#define TP_H 175
#define TP_X 894
#define TP_Y 2000-TP_H

//Teams
typedef enum Team_E{
  GREEN_TEAM,
  ORANGE_TEAM,
}Team;

//Robots
typedef struct Robot_S{
  uint32_t last_detection;
  uint16_t x, y;
  uint16_t speed_x, speed_y;
  uint16_t exists;
}Robot;

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

#endif

