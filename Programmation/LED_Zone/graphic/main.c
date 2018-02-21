#include "song.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fftw3.h>

#define CHUNK_PERIOD 50
#define FREQUENCY_STEP (1000 / CHUNK_PERIOD)
#define BUFFER_SIZE_MS 25
#define MAX(a,b) ((a>=b)?a:b)

static int width = 0;

void drawSpectrum(SDL_Renderer *ren, double fft[], uint32_t len){
  int i, j;
  
  const int wcenter = width/2;
  const int wbar = 4;
  const int hmax = 0.2*width*2;
  const int center = 0.85*width;
  int jmax = width/2;
  if((2+(jmax-1)/wbar) >= (int) len){
    jmax = wbar*(len-1-2);
  }
  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  for(j = 0; j < jmax; j++){
    int h = fft[2+j/wbar]*hmax;
    for(i = center-h; i < center; i++){
      int r = 255*(((float) h-(center-i))/h);
      SDL_SetRenderDrawColor(ren, r*(1.-(float)j/jmax), 0, r*((float)j/jmax), r);
      SDL_RenderDrawPoint(ren, wcenter+j, i);
      SDL_RenderDrawPoint(ren, wcenter+j, center-(i-center)-1);
      SDL_RenderDrawPoint(ren, wcenter-1-j, i);
      SDL_RenderDrawPoint(ren, wcenter-1-j, center-(i-center)-1);
    }
  }
 
}

int main(int argc, char *argv[]){
  if(argc < 2){
    return EXIT_FAILURE;
  }
  
  //Graphics
  SDL_Window *screen;
  SDL_Renderer *renderer;
  SDL_Texture *bgTexture;
  SDL_Surface *bgSurface;
  SDL_Texture *tracksTexture;
  SDL_Surface *tracksSurface;

  //Song
  Song *song;
  Chunk *chunk;
  
  if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  width = dm.h;
  screen = SDL_CreateWindow("JackyLED Factory",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    width, width,
			    SDL_WINDOW_OPENGL);
  if(screen == NULL){
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  
  renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
  

  bgSurface = IMG_Load("./background.jpg");
  bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
  SDL_FreeSurface(bgSurface);

  tracksSurface = IMG_Load("./tracks.png");
  tracksTexture = SDL_CreateTextureFromSurface(renderer, tracksSurface);
  SDL_SetTextureAlphaMod(tracksTexture, 0);
  SDL_FreeSurface(tracksSurface);
  
  song = Load_Song(argv[1], BUFFER_SIZE_MS);
  if(song == NULL){
    return EXIT_FAILURE;
  }

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
  SDL_RenderPresent(renderer);
  SDL_Delay(1000);
  
  Play_Song(song);

  chunk = Make_Chunk(song, CHUNK_PERIOD);
  if(chunk == NULL){
    return EXIT_FAILURE;
  }

  //FFT
  double *in, *result;
  fftw_complex *out;
  fftw_plan p;
  int N = (CHUNK_PERIOD * song->wav_spec.freq)/1000;
  in = (double*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2 + 1));
  p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
  result = malloc(sizeof(*result)*(N/2+1));

  int last_time = 0, current_time = 0;
  int go = 1;
  int update = 0;
  double max = 0.;
  while(Is_Playing_Song(song) && go){

    //Event
    SDL_Event e;
    while(SDL_PollEvent(&e)){
      switch(e.type){
      case SDL_QUIT:
	go = 0;
	break;
      }
    }

    //Rendering
    current_time = SDL_GetTicks();
    if(current_time - last_time > 25 && update){
      last_time = current_time;
      update = 0;
      
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
      SDL_SetTextureAlphaMod(tracksTexture, (uint8_t) chunk->value);
      SDL_RenderCopy(renderer, tracksTexture, NULL, NULL);    
      drawSpectrum(renderer, result, N/2+1);
      SDL_RenderPresent(renderer);
    }

    
#if 0
    uint32_t time = Get_Time_Song(song);
    printf("Time : %d\n", time);
#endif
    
    if(Update_Chunk(chunk)){
      //Process next chunk value
      uint32_t i;
      for(i = 0; i < chunk->len; i++){
	in[i] = chunk->start[i] / 32768.;
      }
      fftw_execute(p);
      for(i = 0; (int) i < (N/2+1); i++){
	result[i] = 2*sqrtf(out[i][0]*out[i][0] + out[i][1]*out[i][1])/N;
      }

      double sum = 0.;
      for(i = 1; i < 11; i++){
	sum += result[i];
      }
      sum /= 10.;
      if(sum > max){
	max = sum;
      }

      chunk->value = (sum/max)*255;
      update = 1;
      
    }
    SDL_Delay(10);
  }
  
  Free_Song(&song);
  Free_Chunk(&chunk);
  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
  SDL_DestroyTexture(bgTexture);
  SDL_DestroyTexture(tracksTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(screen);
  SDL_Quit();
  return EXIT_SUCCESS;
}
