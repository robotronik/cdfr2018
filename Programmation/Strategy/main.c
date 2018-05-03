#include "main.h"
#include "graphics.h"
#include "heap.h"
#include "map.h"
#include "obstacle.h"

void test_heap(){
  Cell a, b, c, d;
  a.g = 1 + (b.g = 1 + (c.g = 1 + (d.g = 0)));
  
  insert(&a);
  insert(&b);
  insert(&d);
  insert(&c);
  Cell *r = pop();
  printf("%d\n", r->g);
  r = pop();
  printf("%d\n", r->g);
  r = pop();
  printf("%d\n", r->g);
  r = pop();
  printf("%d\n", r->g);
}

int main(int argc, char *argv[]){
  (void)argc;
  (void)argv;

  SDL_Window *window;
  SDL_Renderer *renderer;

  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow("Strategy",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    1200, 800,
			    SDL_WINDOW_OPENGL);
  if(window == NULL){
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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

  SDL_Texture *path = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetTextureBlendMode(path, SDL_BLENDMODE_BLEND);

  SDL_Texture *highlight = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetTextureBlendMode(highlight, SDL_BLENDMODE_BLEND);

  SDL_Surface *bg_surface = IMG_Load("./aire_jeu.bmp");
  SDL_Texture *bg = SDL_CreateTextureFromSurface(renderer, bg_surface);
  SDL_FreeSurface(bg_surface);

  //STRATEGY
  Init_Strategy(GREEN_TEAM);
  
  Set_Construction_Plan(ORANGE, GREEN, BLUE);
  //Set_Construction_Plan(GREEN, YELLOW, BLUE);
  //Update_Construction(&cube[BLACK], &current_construction);
  //cube[BLACK].availability = ZERO_PROBABILITY;
  //cube[GREEN].availability = ZERO_PROBABILITY;
  Refresh_Map();
  Compute_Building_Strategy();

  int i;
  for(i = 0; i < nb_targets; i++){
    printf("%s %d\n", color_str[target_list[i].c->color], target_list[i].d);
  }

  me.x = AREA_WIDTH/2 - 400;
  me.y = AREA_HEIGHT/2;
  Update_Obstacles(&me, 1.415*SQUARE_SIZE+1, 0, 0, 0);
  Materialize_Obstacle(&obstacle[0],150);
  Print_Obstacles();

  
  highlight_cubes(renderer, highlight, strat.materials, strat.nb_materials);
  draw_empty_grid(renderer, grid);
  draw_cubes(renderer, cubes);
  draw_cubes_obstacles(renderer, circles);
  draw_robots(renderer, circles, 000);

  SDL_Rect pos_grid;
  pos_grid.x = 0, pos_grid.y = 0;
  SDL_QueryTexture(grid, NULL, NULL, &pos_grid.w, &pos_grid.h);

  Cell *start = &Get_Cell(me.x, me.y);
  int x_goal = MAP_WIDTH-3;
  int y_goal = MAP_HEIGHT-3;
  Cell *end = Compute_Path(start, &map[y_goal][x_goal]);

  if(end == NULL){
    printf("FUUUUUUUUUUUUUUUCK !\n");
  }


  
  draw_grid_obs(renderer, obs);

  draw_path(renderer, path, end);

  

  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF , 0xFF, 0x00);
  SDL_RenderClear(renderer);
  SDL_SetRenderTarget(renderer, NULL);

  #ifdef GRAPHICS
  bool run = true;
  unsigned int last_time, time;
  last_time = time = SDL_GetTicks();
  while(run){
    //Drawing
    SDL_SetRenderDrawColor(renderer, 181, 176, 161, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg, NULL, &pos_grid);
    SDL_RenderCopy(renderer, obs, NULL, &pos_grid);
    SDL_RenderCopy(renderer, grid, NULL, &pos_grid);
    SDL_RenderCopy(renderer, circles, NULL, &pos_grid);
    SDL_RenderCopy(renderer, cubes, NULL, &pos_grid);
    SDL_RenderCopy(renderer, highlight, NULL, &pos_grid);
    SDL_RenderCopy(renderer, path, NULL, &pos_grid);
    SDL_RenderPresent(renderer);
    
    //Events
    SDL_Event e;
    while(SDL_PollEvent(&e)){
      switch(e.type){
      case SDL_QUIT:
	run = false;
	break;
      case SDL_KEYDOWN:
	switch(e.key.keysym.sym){
	case SDLK_RIGHT:
	  if(++x_goal < MAP_WIDTH-1){	    
	    draw_path(renderer, path, Compute_Path(start, &map[y_goal][x_goal]));
	  }else{
	    x_goal = MAP_WIDTH-2;
	  }
	  break;
	case SDLK_LEFT:
	  if(--x_goal > 0){	    
	    draw_path(renderer, path, Compute_Path(start, &map[y_goal][x_goal]));
	  }else{
	    x_goal = 1;
	  }
	  break;
	case SDLK_DOWN:
	  if(++y_goal < MAP_HEIGHT-1){	    
	    draw_path(renderer, path, Compute_Path(start, &map[y_goal][x_goal]));
	  }else{
	    y_goal = MAP_HEIGHT-1;
	  }
	  break;
	case SDLK_UP:
	  if(--y_goal > 0){	    
	    draw_path(renderer, path, Compute_Path(start, &map[y_goal][x_goal]));
	  }else{
	    y_goal = 1;
	  }
	  break;
	}
	break;
      }
    }

    //Delay
    if((time = SDL_GetTicks()) - last_time < DELAY){
      SDL_Delay(DELAY - (time - last_time));
    }
    last_time = SDL_GetTicks();
  }
  #endif

  SDL_DestroyTexture(grid);
  SDL_DestroyTexture(circles);
  SDL_DestroyTexture(cubes);
  SDL_DestroyTexture(obs);
  SDL_DestroyTexture(bg);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
