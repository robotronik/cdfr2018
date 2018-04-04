#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <math.h>
#include <SDL2/SDL.h>
#include "map.h"

#define RATIO 2
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000
#define GRAPHIC_SQUARE_SIZE (SQUARE_SIZE / RATIO)
#define FPS 30
#define DELAY 1000/30

void draw_empty_grid(SDL_Renderer *renderer, SDL_Texture *texture);
void draw_grid_obs(SDL_Renderer *renderer, SDL_Texture *texture, Map *map);
void draw_cubes(SDL_Renderer *renderer, SDL_Texture *texture);
void draw_circle(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color *color, SDL_Rect *rect);
void draw_cubes_obstacles(SDL_Renderer *renderer, SDL_Texture *texture);

#endif
