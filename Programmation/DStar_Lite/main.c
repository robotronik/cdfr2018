#include "main.h"
#include "graphics.h"

int main(int argc, char *argv[]){
  (void)argc;
  (void)argv;

  SDL_Window *window;
  SDL_Renderer *renderer;

  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow("DStar_Lite",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,
			    SDL_WINDOW_OPENGL);
  if(window == NULL){
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_Texture *grid = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetTextureBlendMode(grid, SDL_BLENDMODE_BLEND);
  
  SDL_Texture *cubes = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetTextureBlendMode(cubes, SDL_BLENDMODE_BLEND);
 
   SDL_Texture *circles = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetTextureBlendMode(circles, SDL_BLENDMODE_BLEND);

    SDL_Texture *obs = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetTextureBlendMode(obs, SDL_BLENDMODE_BLEND);
  
  draw_empty_grid(renderer, grid);
  draw_cubes(renderer, cubes);
  draw_cubes_obstacles(renderer, circles);

  SDL_Rect pos_grid;
  pos_grid.x = 0, pos_grid.y = 0;
  SDL_QueryTexture(grid, NULL, NULL, &pos_grid.w, &pos_grid.h);

  Map map;
  init_map(&map);

  draw_grid_obs(renderer, obs, &map);
  
  bool run = true;
  unsigned int last_time, time;
  last_time = time = SDL_GetTicks();
  while(run){
    //Drawing
    SDL_SetRenderDrawColor(renderer, 181, 176, 161, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, obs, NULL, &pos_grid);
    SDL_RenderCopy(renderer, grid, NULL, &pos_grid);
    SDL_RenderCopy(renderer, cubes, NULL, &pos_grid);
    SDL_RenderCopy(renderer, circles, NULL, &pos_grid);
    SDL_RenderPresent(renderer);
    
    //Events
    SDL_Event e;
    while(SDL_PollEvent(&e)){
      switch(e.type){
      case SDL_QUIT:
	run = false;
	break;
      }
    }

    //Delay
    if((time = SDL_GetTicks()) - last_time < DELAY){
      SDL_Delay(DELAY - (time - last_time));
    }
    last_time = SDL_GetTicks();
  }

  SDL_DestroyTexture(grid);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
