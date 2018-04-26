#include "heap.h"
#include "map.h"

#ifdef DEBUG
#include <assert.h>
#endif

#include <stdlib.h>

static int heap_size = 0;
static Cell* heap[HEAP_SIZE];

#define HEAP_FULL (heap_size == HEAP_SIZE)
#define HEAP_EMPTY (heap_size == 0)
#define COMPARE_INDEX(i,j) (heap[i]->f < heap[j]->f)
#define SWAP_INDEX(i,j) {Cell* tmp = heap[i]; heap[i] = heap[j]; heap[j] = tmp;}
#ifdef DEBUG
#define PARENT_INDEX(k) ({assert(k != 0); (k-1)/2;})
#else
#define PARENT_INDEX(k) ((k-1)/2)
#endif
#define LEFT_CHILD_INDEX(k) (2*k+1)
#define RIGHT_CHILD_INDEX(k) (2*k+2)

static inline void percolate_up(int index);
static inline void percolate_down(int index);
static inline int index_max_child(int index);

void empty_heap(){
  heap_size = 0;
}

int is_empty(){
  return heap_size == 0;
}

int is_full(){
  return heap_size == HEAP_SIZE;
}

int insert(Cell *cell){
  if(HEAP_FULL){
    return -1;
  }

  //Insert the element at the end of the heap.
  heap[heap_size] = cell;

  //Percolate up
  percolate_up(heap_size);

  //Update heap size
  heap_size++;
  
  return 0;
}

Cell* pop(){
  if(HEAP_EMPTY){
    return NULL;
  }

  //Get the highest priority element
  Cell *cell = heap[0];

  //Update heap size
  heap_size--;

  if(!HEAP_EMPTY){
    //Move the last element to the first
    heap[0] = heap[heap_size];
    
    //Percolate down
    percolate_down(0);
  }
  
  return cell;
}

Cell* peek(){
  if(HEAP_EMPTY)
    return NULL;
  else
    return heap[0];
}

static inline void percolate_up(int index){
  int k = index;
  int p_index = PARENT_INDEX(k);
  
  while((k > 0) && COMPARE_INDEX(k, p_index)){
    SWAP_INDEX(k, p_index);
    k = p_index;
    p_index = PARENT_INDEX(k);
  }
}

static inline void percolate_down(int index){
  int i_max = index_max_child(index);

  while((i_max != index) && COMPARE_INDEX(i_max, index)){
    SWAP_INDEX(index, i_max);
    index = i_max;
    i_max = index_max_child(index);
  }
}

static inline int index_max_child(int index){
  const int lci = LEFT_CHILD_INDEX(index);
  const int rci = RIGHT_CHILD_INDEX(index);

  if(lci > heap_size - 1)
    return index;

  if(rci > heap_size - 1)
    return lci;
  
  return COMPARE_INDEX(lci, rci)?lci:rci;
}
