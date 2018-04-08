#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdint.h>

//MAP
#define SQUARE_SIZE 50//5cm
#define AREA_HEIGHT 2000
#define AREA_WIDTH 3000
#define MAP_HEIGHT (AREA_HEIGHT / SQUARE_SIZE)
#define MAP_WIDTH (AREA_WIDTH / SQUARE_SIZE)
#define NB_CUBE_SETS 6
#define NB_CUBES (5*NB_CUBE_SETS)
#define CUBE_SIZE 58
#define ROBOT_RADIUS (110 + 20)

//HEAP
#define HEAP_SIZE (MAP_HEIGHT*MAP_WIDTH)

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
#define TP_X 894
#define TP_Y 1800
#define TP_W 1200
#define TP_H 250

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
  LIKELY, //Element on our side
  UNCERTAIN, //Element on the enemy side
  UNLIKELY, //Element on which we saw another robot
  ZERO_PROBABILITY, //Element removed
}Probability;

//Cubes
typedef struct Cube_S{
  Cube_Color color;
  Probability availability;
  uint16_t x, y;
}Cube;

//Cubes sets
#define CUBES_PER_SET (NB_CUBES/NB_CUBE_SETS)
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

//Global variables
extern Team team;
extern Robot me;
extern Robot other_robots[3];
extern Cube cube[NB_CUBES];
extern Cube_Set set[NB_CUBE_SETS];
extern Cell map[MAP_HEIGHT][MAP_WIDTH];

extern uint8_t built_buildings;
extern uint16_t score;

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

//Building strategy


//Time
uint32_t Remaining_Time(void);
/**
 * Returns the remaining time, in ms.
 */

#endif
