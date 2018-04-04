#include "map.h"

static Cube_Set const sets[NB_CUBE_SETS] = {
  {.x = 300, .y = 1190},
  {.x = 850, .y = 540},
  {.x = 1100, .y = 1500},
  {.x = 1900, .y = 1500},
  {.x = 2150, .y = 540},
  {.x = 2700, .y = 1190}
};

#define EXPAND_CUBES_FROM_POS(_x, _y)		\
  {.x = _x - CUBE_SIZE, .y = _y},		\
  {.x = _x, .y = _y},				\
  {.x = _x + CUBE_SIZE, .y = _y},		\
  {.x = _x, .y = _y - CUBE_SIZE},		\
  {.x = _x, .y = _y + CUBE_SIZE}

#define EXPAND_CUBES_FROM_SET_INDEX(i) EXPAND_CUBES_FROM_POS(sets[i].x, sets[i].y)

static Cube cubes[NB_CUBES] = {
  EXPAND_CUBES_FROM_POS(300, 1190),
  EXPAND_CUBES_FROM_POS(850, 540),
  EXPAND_CUBES_FROM_POS(1100, 1500),
  EXPAND_CUBES_FROM_POS(1900, 1500),
  EXPAND_CUBES_FROM_POS(2150, 540),
  EXPAND_CUBES_FROM_POS(2700, 1190),
};

void draw_cubes(SDL_Renderer *renderer, SDL_Texture *texture){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);

  int i;
  for(i = 0; i < NB_CUBES; i++){
    int color = i%5;
    if(cubes[i].x > 1500){
      printf("%d\n", color);
      if(color == GREEN) color = ORANGE;
      else if(color == ORANGE) color = GREEN;
    }
    switch(color){
    case GREEN:
      SDL_SetRenderDrawColor(renderer, 97, 153, 59, 0xFF);
      break;
    case YELLOW:
      SDL_SetRenderDrawColor(renderer, 247, 181, 0, 0xFF);
      break;
    case ORANGE:
      SDL_SetRenderDrawColor(renderer, 208, 93, 40, 0xFF);
      break;
    case BLACK:
      SDL_SetRenderDrawColor(renderer, 14, 14, 16, 0xFF);
      break;
    case BLUE:
      SDL_SetRenderDrawColor(renderer, 0, 124, 176, 0xFF);
      break;      
    }
    SDL_Rect rect = {.x = cubes[i].x / RATIO - (CUBE_SIZE / RATIO)/2, .y = cubes[i].y / RATIO - (CUBE_SIZE / RATIO)/2, .w = CUBE_SIZE / RATIO, .h = CUBE_SIZE / RATIO};
    SDL_RenderFillRect(renderer, &rect);
  }
    
  SDL_SetRenderTarget(renderer, NULL);
}
