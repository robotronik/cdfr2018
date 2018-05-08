#include "build.h"
#include "strategy.h"
#include "gas.h"
#include "z_client.h"
#include <math.h>

static float dir_to_angle[4] = {[FROM_UP] = M_PI/2,
				[FROM_RIGHT] = M_PI,
				[FROM_BOT] = -M_PI/2,
				[FROM_LEFT] = 0};
static void Quit_Set();

void Build_Building(){
  Compute_Building_Strategy();
  //While there is cubes to get
  while(build_ctx.construction.size != 5 && strat.nb_targets){
    //Try to get selected cubes, if it fail try again with another
    //strategy
    int i;
    for(i = 0; i < strat.nb_targets; i++){
      if(Get_Cube(&strat.steps_tab[i]) == -1){
	Compute_Building_Strategy();
	break;
      }
    }
    
    //If the building is complete, stop
    if(i == strat.nb_targets)
      break;
  }
  
  if(build_ctx.on_set){
    Quit_Set();
  }
  
  //If the construction is not empty, place it.
  if(build_ctx.construction.size != 0){
    Place_Building();
  }
}


static void Quit_Set(){
  float last_angle = dir_to_angle[build_ctx.align_dir];
  while(Go_Straight(me.x - DIST_ALIGN * cos(last_angle),
		    me.y - DIST_ALIGN * sin(last_angle),
		    false,
		    1.) != 0);
  build_ctx.on_set = false;
}

int Get_Cube(Target *t){
  Cube *const c = t->c;
  uint8_t set_index = (c-cube)/CUBES_PER_SET;

  float angle_align = dir_to_angle[t->d];
  uint16_t x_align = c->x - DIST_ALIGN * cos(angle_align);
  uint16_t y_align = c->y - DIST_ALIGN * sin(angle_align);

  //Quit set if needed
  if(build_ctx.on_set &&//On a set
     ((build_ctx.set_number != set_index)//But not the right one
      || (build_ctx.align_dir != t->d))){//Or not well aligned){
     Quit_Set();
  }
  
  //Align if needed
  if(!build_ctx.on_set){
    if((GOGOGO(x_align, y_align) == -1)
       || (Rotate(angle_align) == -1)){
      c->availability = UNCERTAIN;//TMP
      return -1;
    }
  }

  //Approach the cube
  Go_Straight(c->x - DIST_APPROACH * cos(angle_align),
	      c->y - DIST_APPROACH * sin(angle_align),
	      true,
	      APPROACH_SPEED_RATIO);
  build_ctx.on_set = true;
  build_ctx.align_dir = t->d;
  build_ctx.set_number = set_index;

  //Stack the cube
  if(Z_Stack(build_ctx.construction.size == 4) == -1){
    return -1;
  }

  c->availability = ZERO_PROBABILITY;
  
  Z_State state;
  if(Z_Wait_State(&state) == -1){
    return -1;
  }
  
  if(state == Z_SUCCESS){
    Update_Construction(c, &build_ctx.construction);
    return 0;
  }else{
    return -1;
  }  
}

void Place_Building(){
  
}
