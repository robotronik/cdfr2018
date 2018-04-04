#include "grid.h"

void draw_empty_grid(SDL_Renderer *renderer, SDL_Texture *texture){
  SDL_SetRenderTarget(renderer, texture);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0x50, 0xFF);
  
  int i;
  for(i = 0; i < (HEIGHT / SQUARE_SIZE); i++){
    SDL_RenderDrawLine(renderer, 0, i*SQUARE_SIZE, WIDTH-1, i*SQUARE_SIZE);
  }

  for(i = 0; i < (WIDTH / SQUARE_SIZE); i++){
    SDL_RenderDrawLine(renderer, i*SQUARE_SIZE, 0, i*SQUARE_SIZE, HEIGHT-1);
  }
  
  SDL_SetRenderTarget(renderer, NULL);
}
