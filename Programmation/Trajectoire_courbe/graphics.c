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
