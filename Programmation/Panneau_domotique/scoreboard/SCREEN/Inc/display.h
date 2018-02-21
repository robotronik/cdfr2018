#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm32f3xx_hal.h"
#include "sprites.h"

extern volatile int refresh;

void wait_refresh(void);
/**
 * Attend jusqu'à ce qu'un rafraîchissement de l'écran soit
 * nécessaire, c'est à dire jusqu'à ce que refresh soit égal à 1.
 */

void display_char(int index, int frame, int position);
/**
 * Affiche la frame 'frame' du caractère d'indice 'index' à la
 * position 'position'.
 */

void write_byte(unsigned char byte);
/**
 * Ecrit l'octet 'byte' sur SERIAL_IN.
 */

#endif
