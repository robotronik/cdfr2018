#include "map.h"

Cube_Set const sets[NB_CUBE_SETS] = {
  {.x = 300, .y = 1190},
  {.x = 850, .y = 540},
  {.x = 1100, .y = 1500},
  {.x = 1900, .y = 1500},
  {.x = 2150, .y = 540},
  {.x = 2700, .y = 1190}
};

#define EXPAND_CUBES_FROM_POS(_x, _y)		\
  {.x = _x - CUBE_SIZE, .y = _y},		\
  {.x = _x, .y = _y},				\
  {.x = _x + CUBE_SIZE, .y = _y},		\
  {.x = _x, .y = _y - CUBE_SIZE},		\
  {.x = _x, .y = _y + CUBE_SIZE}

#define EXPAND_CUBES_FROM_SET_INDEX(i) EXPAND_CUBES_FROM_POS(sets[i].x, sets[i].y)

Cube cubes[NB_CUBES] = {
  EXPAND_CUBES_FROM_POS(300, 1190),
  EXPAND_CUBES_FROM_POS(850, 540),
  EXPAND_CUBES_FROM_POS(1100, 1500),
  EXPAND_CUBES_FROM_POS(1900, 1500),
  EXPAND_CUBES_FROM_POS(2150, 540),
  EXPAND_CUBES_FROM_POS(2700, 1190),
};

Cell map[MAP_HEIGHT][MAP_WIDTH]; 
Cell *start, *goal;

void init_map(){

  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++){
    for(j = 0; j < MAP_WIDTH; j++){
      map[i][j].x = j;
      map[i][j].y = i;
    }
  }

  for(i = 0; i < MAP_HEIGHT; i++){
    map[i][0].obstacle = 1;
    map[i][MAP_WIDTH-1].obstacle = 1;
  }
  for(j = 0; j < MAP_WIDTH; j++){
    map[0][j].obstacle = 1;
    map[MAP_HEIGHT-1][j].obstacle = 1;
    }
  
  
  //Set obstacles
  int n;
  int r = ((CUBE_SIZE / 2) + ROBOT_RADIUS * PADDING)/SQUARE_SIZE + 1;
  for(n = 0; n < NB_CUBES; n++){
    int x0 = cubes[n].x / SQUARE_SIZE;
    int y0 = cubes[n].y / SQUARE_SIZE;
    int x, y;
    for(x = 0; x < r; x++){
      float alpha = acos((float) x / (float) r);
      int y_max = (float) r * sin(alpha) + 1;
      for(y = 0; y < y_max; y++){
	if(x0+x < MAP_WIDTH && x0-x >= 0 && y0+y < MAP_HEIGHT && y0-y >= 0){
	  map[y0+y][x0+x].obstacle = 1;
	  map[y0-y][x0+x].obstacle = 1;
	  map[y0+y][x0-x].obstacle = 1;
	  map[y0-y][x0-x].obstacle = 1;
	}
      }
    }
    x = 11;
    for( y = 10; y < MAP_HEIGHT; y++){
      map[y][x].obstacle = 1;
    }
  }
}

static Cell* get_cell(unsigned x, unsigned y){
  if(x >= MAP_WIDTH || y >= MAP_HEIGHT){
    return NULL;
  }
  return &map[y][x];
}

inline int set_start(unsigned int x, unsigned int y){
  goal = get_cell(x, y);
  return (goal != NULL)?0:-1;
}

inline int set_goal(unsigned int x, unsigned int y){
  start = get_cell(x, y);
  return (start != NULL)?0:-1;
}
