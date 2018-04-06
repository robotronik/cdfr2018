#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include "map.h"

void empty_heap();
/**
 * Set the heap size to 0.
 */

int is_empty();
/**
 * Return 1 if the heap is empty, 0 otherwise.
 */

int is_full();
/**
 * Return 1 if the heap is full, 0 otherwise.
 */

int insert(Cell *cell);
/**
 * Insert the cell to the heap, according to its priority. Returns 0
 * on success, -1 on error.
 */

Cell* pop();
/**
 * Remove the highest priority element from the heap. Returns a
 * pointer on the element on success, NULL on error.
 */

Cell* peek();
/**
 * Return a pointer on the highest priority element on success, NULL
 * on error.
 */ 

#endif
