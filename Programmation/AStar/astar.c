#include <stdlib.h>
#include "astar.h"
#include "heap.h"

#define INFINITE ((uint32_t) ~0)
#define H(s, goal) ({							\
      uint32_t const dx = abs(s->x - goal->x);				\
      uint32_t const dy = abs(s->y - goal->y);				\
      (dx > dy)?((dx-dy)+1.4*dy):((dy-dx)+1.4*dx);			\
    })

static void init_cells();

#define NEIGHBOR_NUMBER 8
static int neighbor_offset[NEIGHBOR_NUMBER] = {
  -MAP_WIDTH-1,
  -MAP_WIDTH,
  -MAP_WIDTH+1,
  -1,
  +1,
  +MAP_WIDTH-1,
  +MAP_WIDTH,
  +MAP_WIDTH+1};

#define NEIGHBOR(p_cell, k) (p_cell+neighbor_offset[k])  
#define CHECK_NEIGHBOR(neighbor) (neighbor-&map[0][0] < (MAP_WIDTH * MAP_HEIGHT) && neighbor >= &map[0][0] && !neighbor->obstacle)

Cell* A_Star(Cell *start, Cell *goal){
  empty_heap();
  init_cells();

  start->g = 0;
  start->f = H(start, goal);
  insert(start);

  while(!is_empty()){
    Cell *current = peek();
    //printf("ok\n");fflush(stdout);
    if(current == goal){
      return goal;
    }

    pop();
    current->evaluated = 1;
    
    int k;
    for(k = 0; k < NEIGHBOR_NUMBER; k++){
      Cell *neighbor = NEIGHBOR(current, k);
      
      if(!CHECK_NEIGHBOR(neighbor) || neighbor->evaluated)
	continue;

      if(current->y == 0 && current->x == 0){
	printf("%d %d %d %d\n", k, neighbor_offset[k], neighbor->x, neighbor->y);
      }
      
      if(neighbor->new_){
	neighbor->new_ = 0;
	insert(neighbor);
      }

      uint32_t g = current->g + 1;
      if(g >= neighbor->g)
	continue;

      neighbor->pred = current;
      neighbor->g = g;
      neighbor->f = neighbor->g + H(neighbor, goal);
    }
  }
  
  return NULL;
}

static void init_cells(){
  int x, y;
  for(y = 0; y < MAP_HEIGHT; y++){
    for(x = 0; x < MAP_WIDTH; x++){
      Cell *const cell = &map[y][x];
      cell->g = cell->f = INFINITE;
      cell->evaluated = 0;
      cell->new_ = 1;
    }
  }
}
