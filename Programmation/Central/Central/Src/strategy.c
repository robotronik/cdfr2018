#include "strategy.h"

#include <stdlib.h>
#include <stdio.h>//TMP
#include <math.h>
#define PI 3.14159265358979323846

#include "map.h"

#define swap(type, a, b) {type tmp = (a); (a) = (b); (b) = tmp;}

Team team;
Robot me;

uint8_t built_buildings;
uint16_t score;

static Cube_Color construction_plan[3];
static uint8_t valid_plan = 0;
static int score_per_size[6] = {0, 1, 1+2, 1+2+3, 1+2+3+4, 1+2+3+4+5};

Construction current_construction;
Target target_list[5];
int nb_targets = 0;

char color_str[5][16] = {
    [GREEN] = "GREEN",
    [YELLOW] = "YELLOW",
    [ORANGE] = "ORANGE",
    [BLACK] = "BLACK",
    [BLUE] = "BLUE"
  };

#define PLAN_TOP construction_plan[0]
#define PLAN_MIDDLE construction_plan[1]
#define PLAN_BOTTOM construction_plan[2]

static int Get_Nb_Cubes_Set(int index_set);
static Cube* Find_Cube(Cube_Color color, uint8_t no_pattern);
static int Find_Cubes(Cube_Color color, uint8_t no_pattern, Cube *to_complete[CUBES_PER_SET], int *current_size, int nmax);
static void Eval_Combination(Cube* comb_ref[], uint8_t mask, float *best_rank);
static void Eval_Permutation(Cube* comb[], int size, float *best_rank);
static float Eval_Move(Robot *robot, uint16_t dest_x, uint16_t dest_y);
static float Eval_Align_Stack(Target *t, Robot *robot);
static float Eval_Place(Robot *robot);
static float Eval_Return_Approach(Robot *robot);

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
//                 Cubes sort                       //
//==================================================//

static Cube *cube_sort[NB_CUBES];
uint16_t from_x, from_y;

static void Init_Cube_Sort(){
  int i;
  for(i = 0; i < NB_CUBES; i++){
    cube_sort[i] = &cube[i];
  }
}

static int Compare_Cubes(const void* a, const void* b){
  Cube *const c_a = *((Cube**)a);
  Cube *const c_b = *((Cube**)b);

  if(c_a->availability < c_b->availability){
    return 1;
  }else if(c_a->availability == c_b->availability){
    float dist_a = dist(from_x, from_y, c_a->x, c_a->y);
    float dist_b = dist(from_x, from_y, c_b->x, c_b->y);
    if(dist_a > dist_b){
      return 1;
    }else if(dist_a == dist_b){
      return 0;
    }
  }
  return -1;
} 


static void Cube_Sort(uint16_t from_x_, uint16_t from_y_){
  from_x = from_x_;
  from_y = from_y_;
  qsort(cube_sort, NB_CUBES, sizeof(Cube*), Compare_Cubes);
}

//==================================================//
//               Init Strategy                      //
//==================================================//

void Init_Strategy(Team _team){
  team = _team;
  me.x = (team == GREEN_TEAM)?ROBOT_X0:(AREA_WIDTH-ROBOT_X0);
  me.y = (team == GREEN_TEAM)?ROBOT_Y0:(AREA_WIDTH-ROBOT_Y0);
  me.angle = (team == GREEN_TEAM)?ROBOT_A0:PI;
  me.on_target = 0;

  //Initializing objects availability
  int k;
  for(k = 0; k < NB_CUBES; k++){
    cube[k].availability = ((_team==GREEN_TEAM) != (k >= (NB_CUBES/2)))?LIKELY:UNCERTAIN;
  }
  for(k = 0; k < NB_SETS; k++){
    set[k].availability = CUBE_SET(k, 0).availability;
  }

  //Map positions
  Init_Map();
    
  Refresh_Map();

  //Construction init
  Init_Construction(&current_construction);
  
  Init_Cube_Sort();
}

void Set_Construction_Plan(Cube_Color bottom, Cube_Color middle, Cube_Color top){
  PLAN_TOP = top;
  PLAN_MIDDLE = middle;
  PLAN_BOTTOM = bottom;
  valid_plan = 1;
}

//==================================================//
//                  Construction                    //
//==================================================//
void Init_Construction(Construction *construction){
  construction->size = 0;
  construction->plan_state = FSM_PLAN_NONE;
}

int Update_Construction(Cube *c, Construction *construction){
  //Check size
  if(construction->size == CUBES_PER_SET){
    return -1;
  }

  //Add cube
  construction->c[construction->size++] = c;

  //Update plan state
  switch(construction->plan_state){
  case FSM_PLAN_NONE:
    if(c->color == PLAN_TOP){
	construction->plan_state = FSM_PLAN_T;
      }else if(c->color == PLAN_BOTTOM){
	construction->plan_state = FSM_PLAN_B;
      }
      break;
    case FSM_PLAN_T:
      if(c->color == PLAN_MIDDLE){
	construction->plan_state = FSM_PLAN_TM;
      }else if(c->color == PLAN_BOTTOM){
	construction->plan_state = FSM_PLAN_B;
      }else if(c->color != PLAN_TOP){
	construction->plan_state = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_B:
      if(c->color == PLAN_MIDDLE){
	construction->plan_state = FSM_PLAN_BM;
      }else if(c->color == PLAN_TOP){
	construction->plan_state = FSM_PLAN_T;
      }else if(c->color != PLAN_BOTTOM){
	construction->plan_state = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_TM:
      if(c->color == PLAN_BOTTOM){
	construction->plan_state = FSM_PLAN_COMPLETE;
      }else if(c->color == PLAN_TOP){
	construction->plan_state = FSM_PLAN_T;
      }else{
	construction->plan_state = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_BM:
      if(c->color == PLAN_TOP){
	construction->plan_state = FSM_PLAN_COMPLETE;
      }else if(c->color == PLAN_BOTTOM){
	construction->plan_state = FSM_PLAN_B;
      }else{
	construction->plan_state = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_COMPLETE:
      break;
  }
  
  return 0;
}

//==================================================//
//               Building Strategy                  //
//==================================================//
Building_Strategy strat;

void Compute_Building_Strategy(){
  strat.nb_materials = Select_Building_Materials(strat.materials);
  //printf("Nb materials : %d\n", strat.nb_materials);
  John_The_Builder();
  //Eval_Move(&me, 205, 450);
  
}

#define CHECK_ACCESSIBILITY(k, color, ncs, mcb, stack) (((CUBE_SET(k, color)).availability >= UNCERTAIN && ((color) != CENTER_COLOR || ((ncs) - (mcb) <= 2)))?&CUBE_SET(k, color):Find_Cube(color, 0))

static int Get_Nb_Cubes_Set(int index_set){
  int nb_cubes_set = 0;
  int i;
  for(i = 0; i < CUBES_PER_SET; i++){
    if(CUBE_SET(index_set, i).availability >= UNCERTAIN){
      nb_cubes_set++;
    }
  }

  return nb_cubes_set;
}

static int Find_Cubes(Cube_Color color, uint8_t no_pattern, Cube *to_complete[CUBES_PER_SET], int *current_size, int nmax){

  int k, n;
  for(k = 0, n = 0; (k < NB_CUBES) && (n < nmax) && (*current_size < CUBES_PER_SET); k++){
    Cube *const current_cube = cube_sort[k];

    //Checking color
    if((no_pattern && ((current_cube->color == PLAN_TOP)
		       || (current_cube->color == PLAN_MIDDLE)
		       || (current_cube->color == PLAN_BOTTOM)))
       || (!no_pattern && current_cube->color != color)
       || (color == CENTER_COLOR && Get_Nb_Cubes_Set((current_cube-&cube[0])/CUBES_PER_SET) > 2)
       || current_cube->availability <= UNLIKELY)
      continue;

    n++;
    to_complete[(*current_size)++] = current_cube;
  }

  return (n == nmax);
}

static Cube* Find_Cube(Cube_Color color, uint8_t no_pattern){
  int k;
  for(k = 0; k < NB_CUBES; k++){
    Cube *const current_cube = cube_sort[k];

    //Checking color
    if((no_pattern && ((current_cube->color == PLAN_TOP)
		       || (current_cube->color == PLAN_MIDDLE)
		       || (current_cube->color == PLAN_BOTTOM)))
       || (!no_pattern && current_cube->color != color)
       || (color == CENTER_COLOR && Get_Nb_Cubes_Set((current_cube-&cube[0])/CUBES_PER_SET) > 2)
       || current_cube->availability <= UNLIKELY)
      continue;

    return current_cube;
  }

  return NULL;
}

int Select_Building_Materials(Cube* materials[CUBES_PER_SET]){
  //Checking current construction
  //printf("Plan state : %d\n", current_construction.plan_state);

  Cube_Set *best_set = &set[0];
  float best_distance = dist(0, 0, AREA_WIDTH, AREA_HEIGHT);
  uint8_t best_is_pattern_compatible = 0;
  int best_cubes_number = 0;
  int best_found = 0;

  int k;
  int nb_found = 0;
  for(k = 0; k < NB_SETS; k++){
    Cube_Set *const current_set = &set[k];
    Cube *found_cubes[CUBES_PER_SET]; nb_found = 0;
    float distance = dist(me.x, me.y, current_set->x, current_set->y);
    
    
    //Sort cubes for finding
    Cube_Sort(current_set->x, current_set->y);
    
    //printf("Checking set %d\n", k);

    if(current_set->availability == ZERO_PROBABILITY) continue;
    
    //Search for plan materials
    uint8_t nb_cubes_set = Get_Nb_Cubes_Set(k);

    uint8_t pattern_compatible = 0;
    if(valid_plan && current_construction.plan_state != FSM_PLAN_COMPLETE){
      uint8_t mcbp;//max cube before pattern
      if(current_construction.plan_state != FSM_PLAN_NONE){
	mcbp = 0;
      }else{
	mcbp = max(0, 2-current_construction.size);
      }

      //Searching for cubes to complete the pattern
      Cube *top, *middle, *bottom;
      switch(current_construction.plan_state){
      case FSM_PLAN_NONE:
	if(current_construction.size > 2) break;
        top = CHECK_ACCESSIBILITY(k, PLAN_TOP, nb_cubes_set, mcbp + 2, stack_set);
	middle = CHECK_ACCESSIBILITY(k, PLAN_MIDDLE, nb_cubes_set, mcbp + 1, stack_set);
	bottom = CHECK_ACCESSIBILITY(k, PLAN_BOTTOM, nb_cubes_set, mcbp + 2, stack_set);
	if(top != NULL && bottom != NULL && middle != NULL){
	  found_cubes[nb_found++] = top;
	  found_cubes[nb_found++] = middle;
	  found_cubes[nb_found++] = bottom;
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_T:
	if(current_construction.size > 3) break;
	bottom = CHECK_ACCESSIBILITY(k, PLAN_BOTTOM, nb_cubes_set, 1, stack_set);
	middle = CHECK_ACCESSIBILITY(k, PLAN_MIDDLE, nb_cubes_set, 0, stack_set);
	if(bottom != NULL && middle != NULL){
	  found_cubes[nb_found++] = bottom;
	  found_cubes[nb_found++] = middle;
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_B:
	if(current_construction.size > 3) break;
	middle = CHECK_ACCESSIBILITY(k, PLAN_MIDDLE, nb_cubes_set, 0, stack_set);
	top = CHECK_ACCESSIBILITY(k, PLAN_TOP, nb_cubes_set, 1, stack_set);
	if(middle != NULL && top != NULL){
	  found_cubes[nb_found++] = middle;
	  found_cubes[nb_found++] = top;
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_BM:
	if(current_construction.size > 4) break;
	top = CHECK_ACCESSIBILITY(k, PLAN_TOP, nb_cubes_set, 0, stack_set);
	if(top != NULL){
	  found_cubes[nb_found++] = top;
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_TM:
	if(current_construction.size > 4) break;
	bottom = CHECK_ACCESSIBILITY(k, PLAN_BOTTOM, nb_cubes_set, 0, stack_set);
	if(bottom != NULL){
	  found_cubes[nb_found++] = bottom;
	  pattern_compatible = 1;
	}
	break;
      default:
	break;
      }
    }else{
      pattern_compatible = 1;
    }

    //Checking pattern compatibility
    if(best_is_pattern_compatible && !pattern_compatible){
      //printf("Ignoring uncompatible set\n");
      continue;
    }

    //Completing set
    Find_Cubes(0, 1, found_cubes, &nb_found, CUBES_PER_SET - current_construction.size - nb_found);

    //Evaluating distances empirically
    
    int i;
    for(i = 0; i < nb_found; i++){
      distance += dist(current_set->x, current_set->y, found_cubes[i]->x, found_cubes[i]->y);
    }
    //printf("Distance : %f\n", distance);

    nb_cubes_set = nb_found;
    //Making a decision
    if(best_is_pattern_compatible && pattern_compatible){
      if(current_set->availability < best_set->availability){
	continue;
      }
      else if(current_set->availability == best_set->availability){
	if(distance > best_distance){
	  continue;
	}else if(distance == best_distance){
	  if(nb_cubes_set <= best_cubes_number){
	    continue;
	  }
	}
      }
    }
    
    /*printf("compatibility : %hhu (%hhu)\n", pattern_compatible, best_is_pattern_compatible);
    printf("availability : %d (%d)\n", current_set->availability, best_set->availability);
    printf("distance : %f (%f)\n", distance, best_distance);
    printf("cubes number : %hhu (%d)\n", nb_cubes_set, best_cubes_number);
    printf("Set is the new best.\n");*/
    
    best_set = current_set;
    for(i = 0; i < nb_found; i++){
      materials[i] = found_cubes[i];
    }

    best_distance = distance;
    best_is_pattern_compatible = pattern_compatible;
    best_cubes_number = nb_cubes_set;
    best_found = nb_found;
    //printf("\n");
  }
  
  return best_found;
}


void John_The_Builder(){
  Cube *comb_ref[5];

  float best_rank = 0;
  
  //Copy selected materials to comb_ref
  int i;
  for(i = 0; i < strat.nb_materials; i++){
    comb_ref[i] = strat.materials[i];
  }
  
  //For each number of cubes possible, generate all combinations
  int n;
  int m = strat.nb_materials;
  for(n = 1; n <= m; n++){
    uint8_t mask = (1 << n) - 1;//As most binary '1' as n.
    uint8_t nb_left;
    uint8_t current;

    if(n == m){
      Eval_Combination(comb_ref, mask, &best_rank);
      continue;
    }
    do{
      //eval
      //printf("%d\n", mask&(~(1<<materials->size)));
      Eval_Combination(comb_ref, mask, &best_rank);
      
      //Count the number of bits "at left"
      current = m-1;
      nb_left = 0;
      while(mask&(1<<current)){
	nb_left++;
	current--;
      }

      //Get the next bit to move
      while(!(mask&(1<<current))){
	current--;
      }
      mask &= ((1<<current)-1);
      mask |= ((1<<(nb_left+1))-1)<<(current+1);

    }while(mask != (((1 << n) - 1) << (m - n)));
    Eval_Combination(comb_ref, mask, &best_rank);
  }
}

static void Eval_Combination(Cube* comb_ref[], uint8_t mask, float *best_rank){
  int i;
  int c[5];
  Cube* comb[5];

  int n = 0;
  //Get size and init first permutation
  for(i = 0; i < 5; i++){
    if(mask&(1<<i)){
      comb[n++] = comb_ref[i];
    }
    c[i] = 0;
  }

  Eval_Permutation(comb, n, best_rank);
    
  i = 0;
  while(i < n){
    if(c[i] < i){
      if(!(i%2)){
	swap(Cube*, comb[0], comb[i]);
      }else{
	swap(Cube*, comb[c[i]], comb[i]);
      }

      Eval_Permutation(comb, n, best_rank);
	
      c[i]++;
      i = 0;
    }else{
      c[i] = 0;
      i++;
    }
  }
}

#define IS_REMOVED(set_index, color) (CUBE_SET((set_index), (color)).availability == ZERO_PROBABILITY)

static int Check_Target(Target *t) {
  int set_i = (t->c-cube)/CUBES_PER_SET;
  switch (t->c->color) {
  case GREEN:
    if (IS_REMOVED(set_i, YELLOW)
	|| (t->d == FROM_RIGHT && team == ORANGE_TEAM)
	|| (t->d == FROM_LEFT && team == GREEN_TEAM))
      return 1;
    break;
  case YELLOW:
    switch (t->d) {
    case FROM_UP:
      if (IS_REMOVED(set_i, BLACK)
	  && IS_REMOVED(set_i, GREEN)
	  && IS_REMOVED(set_i, ORANGE))
	return 1;
      break;
    case FROM_RIGHT:
      if (IS_REMOVED(set_i, BLACK)
	  && IS_REMOVED(set_i, BLUE)
	  && IS_REMOVED(set_i, ((team == GREEN_TEAM) ? ORANGE : GREEN)))
	return 1;
      break;
    case FROM_BOT:
      if (IS_REMOVED(set_i, BLUE)
	  && IS_REMOVED(set_i, ORANGE)
	  && IS_REMOVED(set_i, GREEN))
	return 1;
      break;
    case FROM_LEFT:
      if (IS_REMOVED(set_i, BLUE)
	  && IS_REMOVED(set_i, BLACK)
	  && IS_REMOVED(set_i, ((team == GREEN_TEAM) ? GREEN : ORANGE)))
	return 1;
      break;
    }
    break;
  case ORANGE:
    if (IS_REMOVED(set_i, YELLOW)
	|| (t->d == FROM_LEFT && team == ORANGE_TEAM)
	|| (t->d == FROM_RIGHT && team == GREEN_TEAM))
      return 1;
    break;
  case BLACK:
    if (t->d == FROM_UP || IS_REMOVED(set_i, YELLOW))
      return 1;
    break;
  case BLUE:
    if (t->d == FROM_BOT || IS_REMOVED(set_i, YELLOW))
      return 1;
    break;
  default:
    return 0;
    break;
  }
  
  return 0;
}
 
static float Eval_Target(Target *t, Robot *robot){
  //printf("Position : (%hu, %hu)\n", robot->x, robot->y);
  //printf("Angle : %f\n", robot->angle);
  
  float cost = 0;
  
  if(robot->on_target){
    if((cube-robot->last_target.c)/CUBES_PER_SET == (cube-t->c)/CUBES_PER_SET){
      if(robot->last_target.d == t->d){
	uint8_t ok = 0;
	switch(t->d){
	case FROM_UP:
	case FROM_BOT:
	  if(robot->x == t->c->x){
	    ok = 1;
	  }
	  break;
	case FROM_RIGHT:
	case FROM_LEFT:
	  if(robot->y == t->c->y){
	    ok = 1;
	  }
	  break;
	}
	
	if(ok){
	  cost += Eval_Align_Stack(t, robot);
	  return cost;
	}
      }
      
      cost += Eval_Return_Approach(robot);

      float move = cost += Eval_Move(robot, t->approach_x, t->approach_y);
      if((robot->last_target.d%2) == (t->d%2)){
	cost += 1.5*move;
      }else{
	cost += move;
      }
      
      cost += Eval_Align_Stack(t, robot);
      return cost;
    }

    cost += Eval_Return_Approach(robot);    
  }
  cost += Eval_Move(robot, t->approach_x, t->approach_y);
  cost += Eval_Align_Stack(t, robot);
  
  return cost;
}

static void Eval_Permutation(Cube* comb[], int size, float *best_rank){  
  //Temporary data
  Construction f_const = current_construction;
  Robot f_robot = me;
  
  int score = 0;
  int best_d = -1;
  
  Probability prob_backup[5];
  Target target_list_tmp[5];

  float cost_accum = 0;

  int i;
  //Backup probabilities
  for(i = 0; i < size; i++){
    prob_backup[i] = comb[i]->availability;
  }
  
  for(i = 0; i < size; i++){
    Cube*const c = comb[i];
    
    int d;
    float best_cost = 100000.;
    best_d = -1;
    Robot next_robot;

//printf("----- %s BEGIN -----\n", color_str[c->color]);
    for(d = 0; d < 4; d++){
      Target t = {.c = c, .d = d};
      switch(d){
      case FROM_UP:
	t.approach_x = t.c->x;
	t.approach_y = t.c->y - APPROACH_DIST;
	break;
      case FROM_RIGHT:
	t.approach_x = t.c->x + APPROACH_DIST;
	t.approach_y = t.c->y;
	break;
      case FROM_BOT:
	t.approach_x = t.c->x;
	t.approach_y = t.c->y + APPROACH_DIST;
	break;
      case FROM_LEFT:
	t.approach_x = t.c->x - APPROACH_DIST;
	t.approach_y = t.c->y;
	break;
      }
      if(Check_Target(&t)){
	Robot robot_tmp = f_robot;
	float cost = Eval_Target(&t, &robot_tmp);
	if(cost < best_cost){
	  best_cost = cost;
	  best_d = d;
	  next_robot = robot_tmp;
	  target_list_tmp[i] = t;
	}
      }
    }
//printf("----- %s END -----\n", color_str[c->color]);
    if(best_d == -1)
      break;
    
    Update_Construction(c, &f_const);
    c->availability = ZERO_PROBABILITY;
    f_robot = next_robot;
    cost_accum += best_cost;
    //printf("%d", best_d);
  }
 
  //Return to CZ
  cost_accum += Eval_Place(&f_robot);

  //Restore proba
  for(i = 0; i < size; i++){
    comb[i]->availability = prob_backup[i];
    if(best_d != -1){
      //printf("%s\t", color_str[comb[i]->color]);
    }
  }
  
  if(f_const.plan_state == FSM_PLAN_COMPLETE){
    score += 30;
  }
  score += score_per_size[f_const.size];
  
  if(best_d != -1){
    float rank = (float) score / cost_accum;
    if(rank > *best_rank){
      *best_rank = rank;
      nb_targets = size;
      for(i = 0; i < nb_targets; i++){
	target_list[i] = target_list_tmp[i];
      }
    }
    //printf("Rank : %f (%d)\n", (float)score/cost_accum, score);
  }
}

static float Eval_Align_Stack(Target *t, Robot *robot){
  float angle;
  switch(t->d){
  case FROM_UP:
    robot->x = t->c->x;
    robot->y = t->c->y - ALIGN_DIST;
    angle = PI/2;
    break;
  case FROM_RIGHT:
    robot->x = t->c->x + ALIGN_DIST;
    robot->y = t->c->y;
    angle = PI;
    break;
  case FROM_BOT:
    robot->x = t->c->x;
    robot->y = t->c->y + ALIGN_DIST;
    angle = -PI/2;
    break;
  case FROM_LEFT:
    robot->x = t->c->x - ALIGN_DIST;
    robot->y = t->c->y;
    angle = 0;
    break;
  default:
    //Should not happen
    angle = 0;
    break;
  }
  robot->on_target = 1;
  robot->last_target = *t;

  float cost = ROTATE_COEFF*fabs(angle-robot->angle) + ALIGN_COST + STACK_COST;
  
  robot->angle = angle;
  
  return cost;
}

static float Eval_Move(Robot *robot, uint16_t dest_x, uint16_t dest_y){
  float distance = dist(robot->x, robot->y, dest_x, dest_y);
  float angle_path = angle(robot->x ,robot->y, dest_x, dest_y);
  float delta_angle = fabs(angle_path - robot->angle);

if(delta_angle > PI){
  delta_angle = 2*PI - delta_angle;
 }

  //printf("Going from (%d, %d) to (%d, %d)\tDistance : %f\tInitial angle : %f\tFinal angle : %f\tDelta : %f\tCost : %f + %f = %f\n", robot->x, robot->y, dest_x, dest_y, distance, robot->angle, angle_path, delta_angle, MOVE_COEFF*distance, ROTATE_COEFF*delta_angle, MOVE_COEFF*distance + ROTATE_COEFF*delta_angle);

  robot->x = dest_x;
  robot->y = dest_y;
  robot->angle = angle_path;
	
  return MOVE_COEFF*distance + ROTATE_COEFF*delta_angle;
}

static float Eval_Place(Robot *robot){
  return STACK_COST + Eval_Move(robot, (team==GREEN_TEAM)?CZ_X:(AREA_WIDTH-CZ_X), 0);
}

static float Eval_Return_Approach(Robot *robot){
    float cost = MOVE_COEFF*dist(robot->x, robot->y, robot->last_target.approach_x, robot->last_target.approach_y);
    robot->x = robot->last_target.approach_x;
    robot->y = robot->last_target.approach_y;
    robot->on_target = 0;
return cost;
}
