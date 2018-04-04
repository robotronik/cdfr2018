#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "main.h"

#define SQUARE_SIZE 50//2cm
#define AREA_HEIGHT 2000
#define AREA_WIDTH 3000
#define MAP_HEIGHT (AREA_HEIGHT / SQUARE_SIZE)
#define MAP_WIDTH (AREA_WIDTH / SQUARE_SIZE)
#define NB_CUBE_SETS 6
#define NB_CUBES (5*NB_CUBE_SETS)
#define CUBE_SIZE 58
#define ROBOT_RADIUS 167
#define PADDING 1.2

typedef enum Cube_Color_E{
  GREEN,
  YELLOW,
  ORANGE,
  BLACK,
  BLUE
}Cube_Color;

typedef struct Cube_Set_S{
  int x, y;
}Cube_Set;

typedef struct Cube_S{
  int x, y;
}Cube;

typedef struct Map_S{
  int obs[MAP_HEIGHT][MAP_WIDTH];
}Map;

extern Cube_Set const sets[NB_CUBE_SETS];
extern Cube cubes[NB_CUBES];

void init_map(Map *map);

#endif
