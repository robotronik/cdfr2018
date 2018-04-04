#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "main.h"

#define SQUARE_SIZE (10/RATIO)//1cm
#define MAP_HEIGHT (HEIGHT / SQUARE_SIZE)
#define MAP_WIDTH (WIDTH / SQUARE_SIZE)
#define NB_CUBE_SETS 6
#define NB_CUBES (5*NB_CUBE_SETS)

#define CUBE_SIZE 58

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

void draw_cubes(SDL_Renderer *renderer, SDL_Texture *texture);

#endif
