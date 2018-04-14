#include "graphics.h"


void load_points(Interpol *pinterp)
{
  int i;
  FILE *file;
  if((file=fopen("points.csv","r"))==NULL) {printf("Unable to open points.csv\n");exit(EXIT_FAILURE);}
  fscanf(file,"%d",&pinterp->n);
  printf("Number of points %d\nrank x y\n",pinterp->n);
  for(i=0;i<pinterp->n;i++)
  {
    fscanf(file,"%f %f",&pinterp->x[i],&pinterp->y[i]);
    printf("%d: %f %f\n",i,pinterp->x[i],pinterp->y[i]);
  }
  fclose(file);
}

void clear_window(SDL_Renderer* renderer)
{
  SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = SCREEN_WIDTH;
    r.h = SCREEN_HEIGHT;

  SDL_SetRenderDrawColor( renderer, 255, 255, 255,0 );
  SDL_RenderFillRect( renderer, &r );
}

void trace_points(Interpol *pinterp,SDL_Renderer* renderer)
{
  int i;
  SDL_Rect r;
    r.w = POINT_SIZE;
    r.h = POINT_SIZE;
  SDL_SetRenderDrawColor( renderer, 255, 0, 0,0 );
  for(i=0;i<pinterp->n;i++)
  {
    r.x = pinterp->x[i]-POINT_SIZE/2;
    r.y = pinterp->y[i]-POINT_SIZE/2;
    SDL_RenderFillRect( renderer, &r );
  }
}

void trace_curve(Interpol *pinterp,SDL_Renderer* renderer)
{
  int i;
  SDL_Rect r;
    r.w = 1;
    r.h = 1;
  SDL_SetRenderDrawColor( renderer, 0, 0, 0,0 );
  for(i=0;i<RESULT_LENGTH;i++)
  {
    r.x = pinterp->r_x[i];
    r.y = pinterp->r_y[i];
    SDL_RenderFillRect( renderer, &r );
  }
}

void trace_robot(float robot_x,float robot_y,float robot_theta,SDL_Renderer* renderer)
{
  SDL_Rect r;
    r.w = POINT_SIZE;
    r.h = POINT_SIZE;
  SDL_SetRenderDrawColor( renderer,0, 255, 0,0 );
  r.x = robot_x-POINT_SIZE/2;
  r.y = robot_y-POINT_SIZE/2;
  SDL_RenderFillRect( renderer, &r );
  SDL_RenderDrawLine(renderer,robot_x,robot_y,robot_x+VECTOR_LENGTH*cos(robot_theta),robot_y+VECTOR_LENGTH*sin(robot_theta));
}

void trace_min(Interpol *pinterp,int imin,float thedes,SDL_Renderer*renderer)
{
  SDL_Rect r;
    r.w = POINT_SIZE/2;
    r.h = POINT_SIZE/2;
  SDL_SetRenderDrawColor( renderer,0, 0, 255,0 );
  r.x = pinterp->r_x[imin]-POINT_SIZE/4;
  r.y = pinterp->r_y[imin]-POINT_SIZE/4;
  SDL_RenderFillRect( renderer, &r );
  SDL_RenderDrawLine(renderer,pinterp->r_x[imin]-VECTOR_LENGTH*cos(thedes),pinterp->r_y[imin]-VECTOR_LENGTH*sin(thedes),pinterp->r_x[imin]+VECTOR_LENGTH*cos(thedes),pinterp->r_y[imin]+VECTOR_LENGTH*sin(thedes));
}
