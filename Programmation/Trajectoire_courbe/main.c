#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  SDL_Window *window;
  SDL_Renderer *renderer;

  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow("Curve",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          SCREEN_WIDTH, SCREEN_HEIGHT,
          SDL_WINDOW_OPENGL);
  if(window == NULL){
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  clear_window(renderer);
  SDL_RenderPresent(renderer);

  //BEGIN
  Interpol data;
  load_points(&data);
  trace_points(&data,renderer);

  interpol_calc(&data);
  
  trace_curve(&data,renderer);
  SDL_RenderPresent(renderer);
  //END

  getchar();
  SDL_Quit();
  return EXIT_SUCCESS;
}
