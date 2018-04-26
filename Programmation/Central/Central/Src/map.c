#include <math.h>
#include "map.h"
#include "strategy.h"

Cell map[MAP_HEIGHT][MAP_WIDTH];

#define ADD_PADDING(coord, size, limit) {		\
    if(coord > ROBOT_RADIUS){				\
      coord -= ROBOT_RADIUS;				\
      size += ROBOT_RADIUS;				\
    }else{						\
      coord = 0;					\
    }							\
    if(coord+size+ROBOT_RADIUS < limit){		\
      size += ROBOT_RADIUS;				\
    }else{						\
      size = limit-coord;				\
    }							\
  }

static void square_limit(uint32_t real_x, uint32_t real_y, uint32_t width, uint32_t height){
  ADD_PADDING(real_x, width, AREA_WIDTH);
  ADD_PADDING(real_y, height, AREA_HEIGHT);
  
  const uint8_t x = (float) real_x / SQUARE_SIZE + .5;
  const uint8_t y = (float) real_y / SQUARE_SIZE + .5;
  const uint8_t w = (float) width / SQUARE_SIZE + .5;
  const uint8_t h = (float) height / SQUARE_SIZE + .5;
  
  int i, j;
  for(i = y; i < y+h; i++){
    for(j = x; j < x+w; j++){
      map[i][j].obstacle = 1;
    }
  }
}

static void circle_limit(uint32_t real_x, uint32_t real_y, uint32_t radius){
  radius = radius + ROBOT_RADIUS;
  
  int r = (((float) radius / (float) SQUARE_SIZE) + .5);

  int x0 = real_x / SQUARE_SIZE;
  int y0 = real_y / SQUARE_SIZE;
  int x, y;
  for(x = 0; x < r; x++){
    float alpha = acos((float) x / (float) r);
    int y_max = ((float) r) * sin(alpha) + .5;
    for(y = 0; y < y_max; y++){
      if(x0+x < MAP_WIDTH && x0-x >= 0 && y0+y < MAP_HEIGHT && y0-y >= 0){
	map[y0+y][x0+x].obstacle = 1;
	map[y0-y][x0+x].obstacle = 1;
	map[y0+y][x0-x].obstacle = 1;
	map[y0-y][x0-x].obstacle = 1;
      }
    }
  }

}

void Init_Map(void){
  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++){
    for(j = 0; j < MAP_WIDTH; j++){
      map[i][j].x = j;
      map[i][j].y = i;
    }
  }
}

void Refresh_Map(){
  //Clear
  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++){
    for(j = 0; j < MAP_WIDTH; j++){
      map[i][j].obstacle = 0;
    }
  }

  //Edges
  const int w = ROBOT_RADIUS/SQUARE_SIZE + .5;
  int k, l;
  for(k = 0; k < MAP_HEIGHT; k++){
    for(l = 0; l < w; l++){
      map[k][l].obstacle = 1;
      map[k][MAP_WIDTH-1-l].obstacle = 1;
    }
  }
  for(k = 0; k < MAP_WIDTH; k++){
    for(l = 0; l < w; l++){
      map[l][k].obstacle = 1;
      map[MAP_HEIGHT-1-l][k].obstacle = 1;
    }
    }

  //Cubes
  int n;
  for(n = 0; n < NB_CUBES; n++){
    if(cube[n].availability > ZERO_PROBABILITY)
      circle_limit(cube[n].x, cube[n].y, (CUBE_SIZE*1.4)/2);
  }
  
  //Construction zones
  square_limit(CZ_X, 0, CZ_W, CZ_H);
  square_limit(AREA_WIDTH-CZ_X-CZ_W, 0, CZ_W, CZ_H);

  //Water dispensers
  square_limit(0, WD_SIDE_Y-WD_R/2, WD_R, WD_R);
  square_limit(AREA_WIDTH-1-WD_R, WD_SIDE_Y-WD_R/2, WD_R, WD_R);
  square_limit(WD_BOTTOM_X-WD_R/2, AREA_HEIGHT-1-WD_R, WD_R, WD_R);
  square_limit(AREA_WIDTH-WD_BOTTOM_X-WD_R, AREA_HEIGHT-1-WD_R, WD_R, WD_R);
  
  //Treatment plants
  square_limit(TP_X, TP_Y, TP_W, TP_H);
  
}
