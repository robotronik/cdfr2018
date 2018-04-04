#include "graphics.h"

void draw_empty_grid(SDL_Renderer *renderer, SDL_Texture *texture){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0x50, 0xFF);
  
  int i;
  for(i = 0; i < (SCREEN_HEIGHT / GRAPHIC_SQUARE_SIZE); i++){
    SDL_RenderDrawLine(renderer, 0, i*GRAPHIC_SQUARE_SIZE, SCREEN_WIDTH-1, i*GRAPHIC_SQUARE_SIZE);
  }

  for(i = 0; i < (SCREEN_WIDTH / GRAPHIC_SQUARE_SIZE); i++){
    SDL_RenderDrawLine(renderer, i*GRAPHIC_SQUARE_SIZE, 0, i*GRAPHIC_SQUARE_SIZE, SCREEN_HEIGHT-1);
  }
  
  SDL_SetRenderTarget(renderer, NULL);
}

void draw_grid_obs(SDL_Renderer *renderer, SDL_Texture *texture, Map *map){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++){
    for(j = 0; j < MAP_WIDTH; j++){
      if(map->obs[i][j]){
	SDL_Rect rect = {.x = j*GRAPHIC_SQUARE_SIZE, .y = i*GRAPHIC_SQUARE_SIZE, .w = GRAPHIC_SQUARE_SIZE, .h = GRAPHIC_SQUARE_SIZE};
	SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  
  SDL_SetRenderTarget(renderer, NULL);
}

void draw_circle(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color *color, SDL_Rect *rect){

  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

  int x,y;
  for(x = 0; x < rect->w; x++){
    float alpha = acos((float) x / (float) rect->w);
    float y_max = rect->w * sin(alpha);
    for(y = 0; y < (int) y_max; y++){
      SDL_RenderDrawPoint(renderer, rect->x + x, rect->y + y);
      SDL_RenderDrawPoint(renderer, rect->x + x, rect->y - y);
      SDL_RenderDrawPoint(renderer, rect->x - x, rect->y + y);
      SDL_RenderDrawPoint(renderer, rect->x - x, rect->y - y);
    }
  }
  
  SDL_SetRenderTarget(renderer, NULL);
}

void draw_cubes(SDL_Renderer *renderer, SDL_Texture *texture){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);

  int i;
  for(i = 0; i < NB_CUBES; i++){
    int color = i%5;
    if(cubes[i].x > 1500){
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

void draw_cubes_obstacles(SDL_Renderer *renderer, SDL_Texture *texture){

  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);
  
  int i;
  for(i = 0; i < NB_CUBES; i++){
    SDL_Rect rect;
    rect.x = cubes[i].x / RATIO;
    rect.y = cubes[i].y / RATIO;
    rect.w = rect.h = ((CUBE_SIZE / 2) + (int) (ROBOT_RADIUS * PADDING)) / RATIO;
    SDL_Color c = {.r = 0xFF, .g = 0x00, .b = 0x00, .a = 0x40};
    draw_circle(renderer, texture, &c, &rect);
  }

  SDL_SetRenderTarget(renderer, NULL);
}
