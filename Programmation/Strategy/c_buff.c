#include <stdlib.h>
#include "c_buff.h"


void Empty_Circular_Buffer(Circular_Buffer *buff){
  buff->start = buff->size = 0;
}

int Add_Last(Cube *cube, Circular_Buffer *buff){
  if(buff->size == BUFFER_SIZE){
    return -1;
  }

  buff->data[(buff->start + buff->size)%BUFFER_SIZE] = cube;
  buff->size++;

  return 0;
}

int Add_First(Cube *cube, Circular_Buffer *buff){
  if(buff->size == BUFFER_SIZE){
    return -1;
  }
  
  buff->start = (buff->start - 1)%BUFFER_SIZE;
  buff->data[buff->start] = cube;
  buff->size++;

  return 0;
}

Cube *Remove_First(Circular_Buffer *buff){
  if(buff->size == 0){
    return NULL;
  }

  Cube *tmp = buff->data[buff->start];
  buff->size--;
  buff->start = (buff->start + 1)%BUFFER_SIZE;
  return tmp;
}

Cube *Remove_Last(Circular_Buffer *buff){
  if(buff->size == 0){
    return NULL;
  }

  Cube *tmp = buff->data[buff->size-1];
  buff->size--;
  
  return tmp;
}