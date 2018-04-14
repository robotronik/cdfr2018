#include "strategy.h"

#include <stdlib.h>
#include <stdio.h>//TMP
#include <math.h>

#include "map.h"

#define min(a, b) ((a<b)?a:b)
#define max(a, b) ((a>=b)?a:b)
#define dist(x_a, y_a, x_b, y_b) sqrt(pow(y_b-y_a, 2) + pow(x_b-x_a, 2))
#define swap(type, a, b) {type tmp = (a); (a) = (b); (b) = tmp;}

Team team;
Robot me;
Robot other_robots[3];

uint8_t built_buildings;
uint16_t score;

static Cube_Color construction_plan[3];
static uint8_t valid_plan = 0;

Stack current_stack;

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

static FSM_Plan_State Check_Construction(Stack *stack);
static int Get_Nb_Cubes_Set(int index_set);
static Cube* Find_Cube(Cube_Color color, uint8_t no_pattern);
static int Find_Cubes(Cube_Color color, uint8_t no_pattern, Stack *to_complete, int nmax);
static void Eval_Combination(Cube* comb_ref[], uint8_t mask);
static void Eval_Permutation(Cube* comb[], int size);

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
//                Circular buffer                   //
//==================================================//


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
  Init_Map();

  //Border
  int i, j;
  for(i = 0; i < BORDER_SIZE; i++){
    int j_max = .5 + cos(asin((i+.5)/BORDER_SIZE));
    for(j = 0; j < j_max; j++){
      border[i][j] = 1;
    }
  }
    
  Refresh_Map();

  //Stack init
  Init_Stack(&current_stack);
  
  Init_Cube_Sort();
}



//==================================================//
//               Building Strategy                  //
//==================================================//

void Set_Construction_Plan(Cube_Color bottom, Cube_Color middle, Cube_Color top){
  PLAN_TOP = top;
  PLAN_MIDDLE = middle;
  PLAN_BOTTOM = bottom;
  valid_plan = 1;
}

static FSM_Plan_State Check_Construction(Stack *stack){
  if(!valid_plan){
    return FSM_PLAN_NONE;
  }
  
  FSM_Plan_State fsm_plan = FSM_PLAN_NONE;

  int k = 0; Cube *elt;
  stack_iterator(k, stack, elt){
    switch(fsm_plan){
    case FSM_PLAN_NONE:
      if(elt->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_T;
      }else if(elt->color == PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_B;
      }
      break;
    case FSM_PLAN_T:
      if(elt->color == PLAN_MIDDLE){
	fsm_plan = FSM_PLAN_TM;
      }else if(elt->color == PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_B;
      }else if(elt->color != PLAN_TOP){
	fsm_plan = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_B:
      if(elt->color == PLAN_MIDDLE){
	fsm_plan = FSM_PLAN_BM;
      }else if(elt->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_T;
      }else if(elt->color != PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_TM:
      if(elt->color == PLAN_BOTTOM){
	fsm_plan = FSM_PLAN_COMPLETE;
      }else if(elt->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_T;
      }else{
	fsm_plan = FSM_PLAN_NONE;
      }
      break;
    case FSM_PLAN_BM:
      if(elt->color == PLAN_TOP){
	fsm_plan = FSM_PLAN_COMPLETE;
      }else if(elt->color == PLAN_BOTTOM){
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

static int Find_Cubes(Cube_Color color, uint8_t no_pattern, Stack *to_complete, int nmax){

  int k, n;
  for(k = 0, n = 0; (k < NB_CUBES) && (n < nmax) && (to_complete->size < STACK_SIZE); k++){
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
    Stack_Cube(current_cube, to_complete);    
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

int Select_Building_Materials(Stack *selected){
  //Checking current construction
  FSM_Plan_State plan_state = Check_Construction(&current_stack);
  printf("Plan state : %d\n", plan_state);

  Cube_Set *best_set = &set[0];
  Init_Stack(selected);
  float best_distance = dist(0, 0, AREA_WIDTH, AREA_HEIGHT);
  uint8_t best_is_pattern_compatible = 0;
  int best_cubes_number = 0;


  int k;
  for(k = 0; k < NB_SETS; k++){
    Cube_Set *const current_set = &set[k];
    Stack stack_set;
    float distance = dist(me.x, me.y, current_set->x, current_set->y);
    Init_Stack(&stack_set);
    
    //Sort cubes for finding
    Cube_Sort(current_set->x, current_set->y);
    
    printf("Checking set %d\n", k);

    if(current_set->availability == ZERO_PROBABILITY) continue;
    
    //Search for plan materials
    uint8_t nb_cubes_set = Get_Nb_Cubes_Set(k);

    uint8_t pattern_compatible = 0;
    if(valid_plan && plan_state != FSM_PLAN_COMPLETE){
      uint8_t mcbp;//max cube before pattern
      if(plan_state != FSM_PLAN_NONE){
	mcbp = 0;
      }else{
	mcbp = max(0, 2-current_stack.size);
      }

      //Searching for cubes to complete the pattern
      Cube *top, *middle, *bottom;
      switch(plan_state){
      case FSM_PLAN_NONE:
	if(current_stack.size > 2) break;
        top = CHECK_ACCESSIBILITY(k, PLAN_TOP, nb_cubes_set, mcbp + 2, stack_set);
	middle = CHECK_ACCESSIBILITY(k, PLAN_MIDDLE, nb_cubes_set, mcbp + 1, stack_set);
	bottom = CHECK_ACCESSIBILITY(k, PLAN_BOTTOM, nb_cubes_set, mcbp + 2, stack_set);
	if(top != NULL && bottom != NULL && middle != NULL){
	  Stack_Cube(top, &stack_set);
	  Stack_Cube(middle, &stack_set);
	  Stack_Cube(bottom, &stack_set);
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_T:
	if(current_stack.size > 3) break;
	bottom = CHECK_ACCESSIBILITY(k, PLAN_BOTTOM, nb_cubes_set, 1, stack_set);
	middle = CHECK_ACCESSIBILITY(k, PLAN_MIDDLE, nb_cubes_set, 0, stack_set);
	if(bottom != NULL && middle != NULL){
	  Stack_Cube(bottom, &stack_set);
	  Stack_Cube(middle, &stack_set);
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_B:
	if(current_stack.size > 3) break;
	middle = CHECK_ACCESSIBILITY(k, PLAN_MIDDLE, nb_cubes_set, 0, stack_set);
	top = CHECK_ACCESSIBILITY(k, PLAN_TOP, nb_cubes_set, 1, stack_set);
	if(middle != NULL && top != NULL){
	  Stack_Cube(middle, &stack_set);
	  Stack_Cube(top, &stack_set);
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_BM:
	if(current_stack.size > 4) break;
	top = CHECK_ACCESSIBILITY(k, PLAN_TOP, nb_cubes_set, 0, stack_set);
	if(top != NULL){
	  Stack_Cube(top, &stack_set);
	  pattern_compatible = 1;
	}
	break;
      case FSM_PLAN_TM:
	if(current_stack.size > 4) break;
	bottom = CHECK_ACCESSIBILITY(k, PLAN_BOTTOM, nb_cubes_set, 0, stack_set);
	if(bottom != NULL){
	  Stack_Cube(bottom, &stack_set);
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
      printf("Ignoring uncompatible set\n");
      continue;
    }

    //Completing set
    Find_Cubes(0, 1, &stack_set, STACK_SIZE - current_stack.size - stack_set.size);

    //Evaluating distances empirically
    int i; Cube *elt;
    stack_iterator(i, &stack_set, elt){
      distance += dist(current_set->x, current_set->y, elt->x, elt->y);
    }
    printf("Distance : %f\n", distance);

    nb_cubes_set = stack_set.size;
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
    
    printf("compatibility : %d  (%d)\n", pattern_compatible, best_is_pattern_compatible);
    printf("availability : %d (%d)\n", current_set->availability, best_set->availability);
    printf("distance : %f (%f)\n", distance, best_distance);
    printf("cubes number : %d (%d)\n", nb_cubes_set, best_cubes_number);
    printf("Set is the new best.\n");
    
    best_set = current_set;
    Empty_Stack(selected);
    stack_iterator(i, &stack_set, elt){
      selected->data[i] = stack_set.data[i];
    }
    selected->size = stack_set.size;

    best_distance = distance;
    best_is_pattern_compatible = pattern_compatible;
    best_cubes_number = nb_cubes_set;
    printf("\n");
  }
  
  return 0;
}


Queue* John_The_Builder(Stack *materials){
  Cube *comb_ref[5];

  int k; Cube *elt;
  stack_iterator(k, materials, elt){
    comb_ref[k] = elt;
  }
  
  //For each number of cubes possible, generate all permutations
  int n;
int m = materials->size;
  for(n = 1; n <= materials->size; n++){
    uint8_t mask = (1 << n) - 1;//As most binary '1' as n.
    uint8_t nb_left;
    uint8_t current;

    if(n == m){
      Eval_Combination(comb_ref, mask);
      continue;
    }
    do{
      //eval
      //printf("%d\n", mask&(~(1<<materials->size)));
      Eval_Combination(comb_ref, mask);
      
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

    }while(mask != (((1 << n) - 1) << (materials->size - n)));
    Eval_Combination(comb_ref, mask);
  }

  return materials;
}

static void Eval_Combination(Cube* comb_ref[], uint8_t mask){
  int i;
  int c[5];
  Cube* comb[5];

  int n = 0;
  for(i = 0; i < 5; i++){
    if(mask&(1<<i)){
      comb[n++] = comb_ref[i];
    }
    c[i] = 0;
  }

  Eval_Permutation(comb, n);
    
  i = 0;
  while(i < n){
    if(c[i] < i){
      if(!(i%2)){
	swap(Cube*, comb[0], comb[i]);
      }else{
	swap(Cube*, comb[c[i]], comb[i]);
      }

      Eval_Permutation(comb, n);
	
      c[i]++;
      i = 0;
    }else{
      c[i] = 0;
      i++;
    }
  }
}

typedef enum Direction_E{
  FROM_UP,
  FROM_RIGHT,
  FROM_BOT,
  FROM_LEFT
}Direction;

#define IS_REMOVED(set_index, color) (CUBE_SET((set_index), (color)).availability == ZERO_PROBABILITY)

static void Eval_Permutation(Cube* comb[], int size){
  static char color_str[5][16] = {
    [GREEN] = "GREEN",
    [YELLOW] = "YELLOW",
    [ORANGE] = "ORANGE",
    [BLACK] = "BLACK",
    [BLUE] = "BLUE"
  };
  
  int i;
  int score = 0;
  Stack test;
  Init_Stack(&test);

  Probability prob_backup[5];

  uint8_t ok = 0;
  for(i = 0; i < size; i++){
    Cube*const c = comb[i];
    int set_i = (c-cube)/CUBES_PER_SET;
    //Backup proba
    prob_backup[i] = c->availability;

    //Check if the cube can be taken
    int d;

    ok = 0;
    for(d = 0; d < 4; d++){
      switch(c->color){
      case GREEN:
	if(IS_REMOVED(set_i, YELLOW)
	   || (d == FROM_RIGHT && team == ORANGE_TEAM)
	   || (d == FROM_LEFT && team == GREEN_TEAM))
	  ok = 1;
	break;
      case YELLOW:
	switch(d){
	case FROM_UP:
	  if(IS_REMOVED(set_i, BLACK) 
	     && IS_REMOVED(set_i, GREEN)
	     && IS_REMOVED(set_i, ORANGE))
	    ok = 1;
	  break;
	case FROM_RIGHT:
	  if(IS_REMOVED(set_i, BLACK)
	     && IS_REMOVED(set_i, BLUE)
	     && IS_REMOVED(set_i, ((team==GREEN_TEAM)?ORANGE:GREEN)))
	    ok = 1;
	  break;
	case FROM_BOT:
	  if(IS_REMOVED(set_i, BLUE)
	     && IS_REMOVED(set_i, ORANGE)
	     && IS_REMOVED(set_i, GREEN))
	    ok = 1;
	  break;
	case FROM_LEFT:
	  if(IS_REMOVED(set_i, BLUE)
	     && IS_REMOVED(set_i, BLACK)
	     && IS_REMOVED(set_i, ((team==GREEN_TEAM)?GREEN:ORANGE)))
	    ok = 1;
	  break;
	}
	break;
      case ORANGE:
	if(IS_REMOVED(set_i, YELLOW)
	   || (d == FROM_LEFT && team == ORANGE_TEAM)
	   || (d == FROM_RIGHT && team == GREEN_TEAM))
	  ok = 1;
	break;
      case BLACK:
	if(d == FROM_UP || IS_REMOVED(set_i, YELLOW))
	  ok = 1;
	break;
      case BLUE:
	if(d == FROM_BOT || IS_REMOVED(set_i, YELLOW))
	  ok = 1;
	break;
      }
    }
    if(!ok) break;
    c->availability = ZERO_PROBABILITY;
    
    score += i+1;
    Stack_Cube(c, &test);
  }

  //Restore proba
  for(i = 0; i < size; i++){
    if(ok)
      printf("%s\t", color_str[comb[i]->color]);
    comb[i]->availability = prob_backup[i];
  }
  
  FSM_Plan_State fsm_plan = Check_Construction(&test);
  if(fsm_plan == FSM_PLAN_COMPLETE){
    score += 30;
  }
  printf("%d\n", score);
   
 }
