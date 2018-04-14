#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <math.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpol.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define POINT_SIZE 10

void load_points(Interpol *pinterp);
void clear_window(SDL_Renderer* renderer);
void trace_points(Interpol *pinterp,SDL_Renderer* renderer);
void trace_curve(Interpol *pinterp,SDL_Renderer* renderer);

#endif
