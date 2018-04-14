#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <math.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpol.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define POINT_SIZE 10
#define VECTOR_LENGTH 20

void load_points(Interpol *pinterp);
void clear_window(SDL_Renderer* renderer);
void trace_points(Interpol *pinterp,SDL_Renderer* renderer);
void trace_robot(float robot_x,float robot_y,float robot_theta,SDL_Renderer* renderer);
void trace_curve(Interpol *pinterp,SDL_Renderer* renderer);
void trace_min(Interpol *pinterp,int imin,float thedes,SDL_Renderer*renderer);

#endif
