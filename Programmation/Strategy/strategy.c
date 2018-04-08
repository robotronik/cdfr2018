#include "strategy.h"

#include <stdlib.h>
#include <math.h>

Team team;
Robot me;
Robot other_robots[3];
Cell map[MAP_HEIGHT][MAP_WIDTH];

uint8_t built_buildings;
uint16_t score;

static Cube_Color construction_plan[3];
static uint8_t valid_plan = 0;

static Cube* stack[5];
static uint8_t stack_start = 0;
static uint8_t stack_size = 0;

//==================================================//
//                     Cubes                        //
//==================================================//
#define EXPAND_CUBES_FROM_POS(_x, _y, _side)				\
  {.x = _x - CUBE_SIZE, .y = _y, .color = (_side==GREEN_TEAM)?GREEN:ORANGE}, \
  {.x = _x, .y = _y, .color = YELLOW},					\
  {.x = _x + CUBE_SIZE, .y = _y, .color = (_side==GREEN_TEAM)?ORANGE:GREEN}, \
  {.x = _x, .y = _y - CUBE_SIZE, .color = BLACK},			\
  {.x = _x, .y = _y + CUBE_SIZE, .color = BLUE}

Cube cube[NB_CUBES] = {
  EXPAND_CUBES_FROM_POS(300, 1190, GREEN_TEAM),
  EXPAND_CUBES_FROM_POS(850, 540, GREEN_TEAM),
  EXPAND_CUBES_FROM_POS(1100, 1500, GREEN_TEAM),
  EXPAND_CUBES_FROM_POS(1900, 1500, ORANGE_TEAM),
  EXPAND_CUBES_FROM_POS(2150, 540, ORANGE_TEAM),
  EXPAND_CUBES_FROM_POS(2700, 1190, ORANGE_TEAM),
};

//==================================================//
//                  Cubes sets                      //
//==================================================//
Cube_Set set[NB_SETS] = {
  {.x = 300, .y = 1190},
  {.x = 850, .y = 540},
  {.x = 1100, .y = 1500},
  {.x = 1900, .y = 1500},
  {.x = 2150, .y = 540},
  {.x = 2700, .y = 1190}
};

//==================================================//
//               Init Strategy                      //
//==================================================//

#define BORDER_SIZE (ROBOT_RADIUS/SQUARE_SIZE + ((ROBOT_RADIUS%SQUARE_SIZE > SQUARE_SIZE/2)?1:0))
static uint8_t border[BORDER_SIZE][BORDER_SIZE];

void Init_Strategy(Team _team){
  team = _team;
  me.x = (team == GREEN_TEAM)?ROBOT_X0:(AREA_WIDTH-ROBOT_X0);
  me.y = (team == GREEN_TEAM)?ROBOT_Y0:(AREA_WIDTH-ROBOT_Y0);

  //Initializing objects availability
  int k;
  for(k = 0; k < NB_CUBES; k++){
    cube[k].availability = ((_team==GREEN_TEAM) != (k >= (NB_CUBES/2)))?LIKELY:UNCERTAIN;
  }
  for(k = 0; k < NB_SETS; k++){
    set[k].availability = CUBE_SET(k, 0).availability;
  }

  //Map positions
  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++){
    for(j = 0; j < MAP_WIDTH; j++){
      map[i][j].x = j;
      map[i][j].y = i;
    }
  }

  //Border
  for(i = 0; i < BORDER_SIZE; i++){
    int j_max = .5 + cos(asin((i+.5)/BORDER_SIZE));
    for(j = 0; j < j_max; j++){
      border[i][j] = 1;
    }
  }
    
  Refresh_Map();
}

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

//==================================================//
//               Building Strategy                  //
//==================================================//
void Set_Construction_Plan(Cube_Color bottom, Cube_Color middle, Cube_Color top){
  construction_plan[0] = top;
  construction_plan[1] = middle;
  construction_plan[2] = bottom;
  valid_plan = 1;
}

#define PLAN_TOP construction_plan[0]
#define PLAN_MIDDLE construction_plan[1]
#define PLAN_BOTTOM construction_plan[2]

typedef enum FSM_Plan_State_E{
  FSM_PLAN_NONE,
  FSM_PLAN_T,
  FSM_PLAN_TM,
  FSM_PLAN_B,
  FSM_PLAN_BM,
  FSM_PLAN_COMPLETE
}FSM_Plan_State;

#define STACK(k) stack[(stack_start+k)%5]

static FSM_Plan_State Check_Construction(){
  if(!valid_plan){
    return FSM_PLAN_NONE;
  }
  
  FSM_Plan_State fsm_plan = FSM_PLAN_NONE;
  int k;
  for(k = 0; k < stack_size; k++){
    switch(fsm_plan){
    case FSM_PLAN_NONE:
      if(STACK(k)->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_T;
      }else if(STACK(k)->color == PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_B;
      }
      break;
    case FSM_PLAN_T:
      if(STACK(k)->color == PLAN_MIDDLE){
	fsm_plan = FSM_PLAN_TM;
      }else if(STACK(k)->color == PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_B;
      }else if(STACK(k)->color != PLAN_TOP){
	fsm_plan = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_B:
      if(STACK(k)->color == PLAN_MIDDLE){
	fsm_plan = FSM_PLAN_BM;
      }else if(STACK(k)->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_T;
      }else if(STACK(k)->color != PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_TM:
      if(STACK(k)->color == PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_COMPLETE;
      }else if(STACK(k)->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_T;
      }else{
	fsm_plan = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_BM:
      if(STACK(k)->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_COMPLETE;
      }else if(STACK(k)->color == PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_B;
      }else{
	fsm_plan = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_COMPLETE:
      return FSM_PLAN_COMPLETE;
      break;
    }
  }
  
  return fsm_plan;
}

int Select_Building_Materials(){
  Cube_Set *best_set = &set[0];
  int k;

  //Checking current construction
  FSM_Plan_State plan_state = Check_Construction();
  
  for(k = 1; k < NB_SETS; k++){
    Cube_Set *const current_set = &set[2];

    //Check for plan materials
    if(valid_plan){
      /*
       * TODO : function that count the number of plan materials that
       * can complete the current stack. Take them into account only
       * if they will complete the building. Sometimes the needed
       * cubes are there but can't be taken in the right order (often
       * when the yellow is needed).
      */
    }
    
    //Check for availability
    if(current_set->availability > best_set->availability){
      best_set = current_set;
      break;
    }

    //Check for cubes number
    //Do not take cubes that cannot be stacked into account, prefer a
    //set with the right number of cubes.
    
    //Check for distance
    
  }

  //Once the set is selected, do NOT take the pattern's cubes that
  //can't lead to a complete building.
  
  return 0;
}
