#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"

int main(int argc, char *argv[])
{
  int imin;

  SDL_Window *window;
  SDL_Renderer *renderer;

  if(argc!=3)
  {
    printf("enter x and y of the robot\n");
    return EXIT_FAILURE;
  }

  float robot_x=atof(argv[1]),robot_y=atof(argv[2]);

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

  float robot_theta=0,thedes,flexion,kc;
  //BEGIN
  Interpol data;
  load_points(&data);
  trace_points(&data,renderer);

  interpol_calc(&data);

  trace_robot(robot_x,robot_y,robot_theta,renderer);
  trace_curve(&data,renderer);
  SDL_RenderPresent(renderer);

  kc=Kc(&data,1,1,robot_x,robot_y,robot_theta,&imin,&thedes,&flexion);

  printf("Imin:%d theta_res:%f flexion:%f Kc:%f\n",imin,thedes,flexion,kc);
  trace_min(&data,imin,thedes,renderer);
  SDL_RenderPresent(renderer);
  //END

  getchar();
  SDL_Quit();
  return EXIT_SUCCESS;
}
