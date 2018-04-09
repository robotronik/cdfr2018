#ifndef STRATEGY_H
#define STRATEGY_H

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

//HEAP
#define HEAP_SIZE (MAP_HEIGHT*MAP_WIDTH)

//Stack
#define STACK_SIZE 5

//ROBOT
#define ROBOT_X0 200
#define ROBOT_Y0 450

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

//Cubes sets
#define CUBES_PER_SET (NB_CUBES/NB_SETS)
#define CUBE_SET(set, n) (cube[set+n])
typedef struct Cube_Set_S{
  uint16_t x, y;
  Probability availability;
}Cube_Set;

//Cells for path finding
typedef struct Cell_S{
  struct Cell_S *pred;
  uint8_t obstacle;
  uint8_t x, y;
  uint8_t evaluated, new_;
  uint16_t g;
  uint16_t f;
}Cell;

//Stack
typedef struct Stack_S{
  Cube* data[STACK_SIZE];
  uint8_t start;
  uint8_t size;
}Stack;

//Global variables
extern Team team;
extern Robot me;
extern Robot other_robots[3];
extern Cube cube[NB_CUBES];
extern Cube_Set set[NB_SETS];
extern Cell map[MAP_HEIGHT][MAP_WIDTH];

extern uint8_t built_buildings;
extern uint16_t score;
extern Stack current_stack;

//Init
void Init_Strategy(Team team);

//Map functions
void Refresh_Map(void);
/**
 * Refresh the map with the known obstacles, ie. table elements,
 * edges, robots, cubes and buildings.
 */

#define Get_Cell(x, y) (map[(y==AREA_HEIGHT)?MAP_HEIGHT-1:y/SQUARE_SIZE][(x==AREA_WIDTH)?MAP_WIDTH-1:x/SQUARE_SIZE])

//Path planning
#define Compute_Path(start, goal) A_Star(start, goal)
Cell* A_Star(Cell *start, Cell *goal);
/**
 * Computes a shortest path according to the A* algorithm.  Returns a
 * list of the cells constituing the path on success, NULL on error.
 * start and goal must NOT be on any edges of the map.
 */

//Stack Management
#define Init_Stack() Empty_Stack()
void Empty_Stack(Stack *stack);
/**
 * Clear the stack.
 */

int Stack_Cube(Cube *cube, Stack *stack);
/**
 * Add the cube '*cube' to the stack '*stack' if the stack is not full
 * and return 0, however do nothing and return -1.
 */

Cube* Unstack_Cube(Stack *stack);
/**
 * Return a pointer on the first cube in the stack if the stack is not
 * empty, however return NULL.
 */

//Building strategy
void Set_Construction_Plan(Cube_Color bottom, Cube_Color middle, Cube_Color top);
/**
 * This function stores the construction plan for further use in
 * strategy.
 */

int Select_Building_Materials();
/**
 * This function selects cubes in the map to complete the current
 * stack. It returns 0 on success, -1 if no cubes were found.
 */

Cube* John_The_Builder();
/**
 * This function apply a brute force on the selected building
 * materials to make the best decision possible. It returns a sorted
 * list of the cubes to retrieve. Please note that this list may be
 * empty : it means that either there are no more materials left,
 * either the best action to do now is to place the current building
 * on the construction zone.
 */

//void Stack_Cube(Cube *cube);
//void Unstack_Cube();
//void Remove_Cube(Cube *cube);

//Time
uint32_t Remaining_Time(void);
/**
 * Returns the remaining time, in ms.
 */

#endif
