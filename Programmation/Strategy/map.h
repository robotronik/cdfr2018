#ifndef MAP_H
#define MAP_H

#include "game_defs.h"

//Cells for path finding
typedef struct Cell_S{
  struct Cell_S *pred;
  uint8_t obstacle;
  uint8_t x, y;
  uint8_t evaluated, new_;
  uint16_t g;
  uint16_t f;
}Cell;

void Init_Map(void);
/**
 * Init the map by setting the positions x and y.
 */

void Refresh_Map(void);
/**
 * Refresh the map with the known obstacles, ie. table elements,
 * edges, robots, cubes and buildings.
 */

//Path planning
#define Compute_Path(start, goal) A_Star(start, goal)
Cell* A_Star(Cell *start, Cell *goal);
/**
 * Computes a shortest path according to the A* algorithm.  Returns a
 * list of the cells constituing the path on success, NULL on error.
 * start and goal must NOT be on any edges of the map.
 */

extern Cell map[MAP_HEIGHT][MAP_WIDTH];

#endif

