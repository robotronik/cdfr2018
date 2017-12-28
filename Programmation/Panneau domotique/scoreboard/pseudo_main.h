#ifndef PSEUDO_MAIN_H
#define PSEUDO_MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "sprites.h"

#define NB_DIGITS 3
#define MAX_SCORE 999

void get_sprite_index(uint16_t score, uint8_t index[NB_DIGITS]);
void print_frame(uint8_t frame, uint8_t index, uint8_t offset);
void blank_row(uint8_t row);

#endif
