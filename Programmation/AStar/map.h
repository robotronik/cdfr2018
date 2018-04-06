#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "main.h"
#include "config.h"

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

typedef struct Cell_S{
  struct Cell_S *pred;
  short obstacle;
  int x, y;
  int evaluated, new_;
  uint32_t g;
  uint32_t f;
}Cell;

extern Cube_Set const sets[NB_CUBE_SETS];
extern Cube cubes[NB_CUBES];

extern Cell map[MAP_HEIGHT][MAP_WIDTH];
extern Cell *start, *goal;
extern int mapiteration;

void init_map();
int set_start(unsigned int x, unsigned int y);
int set_goal(unsigned int x, unsigned int y);

#endif
