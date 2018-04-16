#ifndef C_BUFF_H
#define C_BUFF_H

#include "game_defs.h"

#define BUFFER_SIZE CUBES_PER_SET
#define STACK_SIZE BUFFER_SIZE
#define QUEUE_SIZE BUFFER_SIZE

typedef struct Circular_Buffer_S{
  Cube* data[BUFFER_SIZE];
  uint8_t start;
  uint8_t size;
}Circular_Buffer;

typedef Circular_Buffer Stack;
typedef Circular_Buffer Queue;

//Circular buffer
void Empty_Circular_Buffer(Circular_Buffer *buff);
int Add_Last(Cube *cube, Circular_Buffer *buff);
int Add_First(Cube *cube, Circular_Buffer *buff);
Cube *Remove_First(Circular_Buffer *buff);
Cube *Remove_Last(Circular_Buffer *buff);

//Stack Management
#define Init_Stack(p_stack) Empty_Stack((p_stack))
#define Empty_Stack(p_stack) Empty_Circular_Buffer((p_stack))
/**
 * Clear the stack.
 */

#define Stack_Cube(p_cube, p_stack) Add_Last((p_cube), (p_stack))
/**
 * Add the cube '*cube' to the stack '*stack' if the stack is not full
 * and return 0, however do nothing and return -1.
 */

#define Unstack_Cube(p_stack) Remove_Last((p_stack))
/**
 * Return a pointer on the first cube in the stack if the stack is not
 * empty, however return NULL.
 */

#define buff_iterator(k, p_stack, p_elt) for((k) = 0, (p_elt) = (p_stack)->data[(p_stack)->start]; (k) < (p_stack)->size; (k)++, (p_elt) = (p_stack)->data[((p_stack)->start + (k))%STACK_SIZE])

//Queue Management
#define Init_Queue(p_queue) Empty_Queue((p_queue))
#define Empty_Queue(p_queue) Empty_Circular_Buffer((p_queue))
/**
 * Clear the queue.
 */

#define Enqueue_Cube(p_cube, p_queue) Add_Last((p_cube), (p_queue))
/**
 * Add the cube '*cube' to the queue '*queue' if the queue is not full
 * and return 0, however do nothing and return -1.
 */

#define Dequeue_Cube(p_queue) Remove_First((p_queue))
/**
 * Return a pointer on the first cube in the queue if the queue is not
 * empty, however return NULL.
 */

#endif

