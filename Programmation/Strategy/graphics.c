#include "graphics.h"
#include "map.h"
#include "obstacle.h"

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

void draw_grid_obs(SDL_Renderer *renderer, SDL_Texture *texture){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xC0);

  int i, j;
  for(i = 0; i < MAP_HEIGHT; i++){
    for(j = 0; j < MAP_WIDTH; j++){
      if(map[i][j].obstacle){
	SDL_Rect rect = {.x = j*GRAPHIC_SQUARE_SIZE, .y = i*GRAPHIC_SQUARE_SIZE, .w = GRAPHIC_SQUARE_SIZE, .h = GRAPHIC_SQUARE_SIZE};
	SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  
  SDL_SetRenderTarget(renderer, NULL);
}

void draw_path(SDL_Renderer *renderer, SDL_Texture *texture, Cell *goal){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);

  if(goal != NULL){
    SDL_SetRenderDrawColor(renderer, 0x40, 0x40, 0xFF, 0xD0);

    Cell *tmpCell;
    for (tmpCell = goal; tmpCell != NULL; tmpCell = tmpCell->pred){
      SDL_Rect rect = {.x = tmpCell->x*GRAPHIC_SQUARE_SIZE, .y = tmpCell->y*GRAPHIC_SQUARE_SIZE, .w = GRAPHIC_SQUARE_SIZE, .h = GRAPHIC_SQUARE_SIZE};
      SDL_RenderFillRect(renderer, &rect);
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

  int i,j;
  for(i = 0; i < NB_SETS; i++){
    for(j = 0; j < CUBES_PER_SET; j++){
      Cube *c = &cube[i*CUBES_PER_SET+j];
      if(c->availability == ZERO_PROBABILITY)
	continue;
      uint8_t alpha = (c->availability==LIKELY)?0xFF:0xA0;
      switch(c->color){
      case GREEN:
	SDL_SetRenderDrawColor(renderer, 97, 153, 59, alpha);
	break;
      case YELLOW:
	SDL_SetRenderDrawColor(renderer, 247, 181, 0, alpha);
	break;
      case ORANGE:
	SDL_SetRenderDrawColor(renderer, 208, 93, 40, alpha);
	break;
      case BLACK:
	SDL_SetRenderDrawColor(renderer, 14, 14, 16, alpha);
	break;
      case BLUE:
	SDL_SetRenderDrawColor(renderer, 0, 124, 176, alpha);
	break;      
      }
      SDL_Rect rect = {.x = c->x / RATIO - (CUBE_SIZE / RATIO)/2, .y = c->y / RATIO - (CUBE_SIZE / RATIO)/2, .w = CUBE_SIZE / RATIO, .h = CUBE_SIZE / RATIO};
      SDL_RenderFillRect(renderer, &rect);
    }
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
    rect.x = cube[i].x / RATIO;
    rect.y = cube[i].y / RATIO;
    rect.w = rect.h = ((CUBE_SIZE / 2) + (int) (ROBOT_RADIUS)) / RATIO;
    SDL_Color c = {.r = 0xFF, .g = 0x00, .b = 0x00, .a = 0x40};
    draw_circle(renderer, texture, &c, &rect);
  }

  SDL_SetRenderTarget(renderer, NULL);
}

void draw_robots(SDL_Renderer *renderer, SDL_Texture *texture){
  SDL_SetRenderTarget(renderer, texture);
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  
  SDL_Rect rect;
  rect.x = me.x / RATIO;
  rect.y = me.y / RATIO;
  rect.w = rect.h = ((int) (ROBOT_RADIUS)) / RATIO;
  SDL_Color c = {.r = 0x00, .g = 0xFF, .b = 0x00, .a = 0xA0};
  draw_circle(renderer, texture, &c, &rect);

  int i;
  for(i = 0; i < nb_obstacles; i++){
    Obstacle *const obs = &obstacle[i];
    rect.x = obs->x_c / RATIO;
    rect.y = obs->y_c / RATIO;
    rect.w = rect.h = ((int) OBS_RADIUS + ROBOT_RADIUS + 100) / RATIO;
    draw_circle(renderer, texture, &c, &rect);
  }

  SDL_SetRenderTarget(renderer, NULL);
}

void highlight_cubes(SDL_Renderer *renderer, SDL_Texture *texture, Cube *selected[CUBES_PER_SET], int n){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);
  
  
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
  int i;
  for(i = 0; i < n; i++){
    SDL_Rect rect = {.x = selected[i]->x / RATIO - (CUBE_SIZE / RATIO)/2, .y = selected[i]->y / RATIO - (CUBE_SIZE / RATIO)/2, .w = CUBE_SIZE / RATIO, .h = CUBE_SIZE / RATIO};
    SDL_SetRenderDrawColor(renderer, 0x40, 0xFF, 0x40, 0xFF);
    SDL_RenderFillRect(renderer, &rect);

    rect.x += 5;
    rect.y += 5;
    rect.w -= 10;
    rect.h -= 10;
    SDL_SetRenderDrawColor(renderer, 0xA0, 0xFF, 0xA0, 0x00);
    SDL_RenderFillRect(renderer, &rect);
  }
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
  SDL_SetRenderTarget(renderer, NULL);
}
