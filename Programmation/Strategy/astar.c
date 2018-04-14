#include <stdlib.h>
#include <math.h>

#include "strategy.h"
#include "heap.h"
#include "map.h"


#define D_ADJ 10
#define D_DIAG 18

#define INFINITE ((uint16_t) ~0)
#define max(a,b) (((a)>(b))?(a):(b))

//Chebyshev distance
#define H(s, goal) ({							\
      uint32_t const dx = abs(s->x - goal->x);				\
      uint32_t const dy = abs(s->y - goal->y);				\
      ((dx > dy)?(10*(dx-dy)+14*dy):(10*(dy-dx)+14*dx));		\
      })

//sqrt(pow(s->x - goal->x, 2) + pow(s->y - goal->y, 2))
//max(abs((goal)->x - (s)->x), abs((goal)->y - (s)->y))

static void init_cells();

#define NEIGHBOR_NUMBER 8
static int neighbor_offset[NEIGHBOR_NUMBER] = {
  -MAP_WIDTH,
  -1,
  +1,
  +MAP_WIDTH,
  
  -MAP_WIDTH-1,
  -MAP_WIDTH+1,
  +MAP_WIDTH-1,
  +MAP_WIDTH+1};

#define NEIGHBOR(p_cell, k) (p_cell+neighbor_offset[k])  
#define CHECK_NEIGHBOR(neighbor) (neighbor-&map[0][0] < (MAP_WIDTH * MAP_HEIGHT) && neighbor >= &map[0][0] && !neighbor->obstacle)

Cell* A_Star(Cell *start, Cell *goal){
  empty_heap();
  init_cells();

  start->g = 0;
  start->f = H(start, goal);
  insert(start);

  uint32_t max = 0;
  while(!is_empty()){
    Cell *current = peek();

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
      
      if(neighbor->new_){
	neighbor->new_ = 0;
	insert(neighbor);
      }

      uint32_t g = current->g + ((k<4)?D_ADJ:D_DIAG);
      if(g >= neighbor->g)
	continue;

      neighbor->pred = current;
      neighbor->g = g;
      neighbor->f = neighbor->g + H(neighbor, goal);
      if(neighbor->f > max){
	max = neighbor->f;	
      }
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
      cell->pred = NULL;
    }
  }
}
