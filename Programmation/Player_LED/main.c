#include "song.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fftw3.h>

void drawSpectrum(SDL_Renderer *ren, double fft[], uint32_t len){
  int i, j;
  const int center = 880;
  SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
  for(j = 0; j < 1080; j++){
    int h = 2*fft[1+j/8]*255;
    for(i = center-h; i < center; i++){
      int r = 255*(((float) h-(center-i))/h);
      SDL_SetRenderDrawColor(ren, r, 0, 0, r);
      SDL_RenderDrawPoint(ren, j, i);
      SDL_RenderDrawPoint(ren, j, center-(i-center)-1);
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

  screen = SDL_CreateWindow("JackyLED Factory",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    1080, 1080,
			    SDL_WINDOW_OPENGL);
  renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);

  bgSurface = IMG_Load("./background.jpg");
  bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
  SDL_FreeSurface(bgSurface);

  tracksSurface = IMG_Load("./tracks.png");
  tracksTexture = SDL_CreateTextureFromSurface(renderer, tracksSurface);
  SDL_SetTextureAlphaMod(tracksTexture, 0);
  SDL_FreeSurface(tracksSurface);
  
  song = Load_Song(argv[1], 25);
  if(song == NULL){
    return EXIT_FAILURE;
  }

  SDL_Delay(1000);
  Play_Song(song);

  chunk = Make_Chunk(song, 50);
  if(chunk == NULL){
    return EXIT_FAILURE;
  }

  //FFT
  double *in, *result;
  fftw_complex *out;
  fftw_plan p;
  int N = (50 * song->wav_spec.freq)/1000;
  in = (double*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2 + 1));
  p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
  result = malloc(sizeof(*result)*(N/2+1));

  int last_time = 0, current_time = 0;
  int go = 1;
  double max = 0.;
  while(Is_Playing_Song(song) && go){

    //Event
    SDL_Event e;
    while(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT){
        go = 0;
      }
    }

    //Rendering
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    SDL_SetTextureAlphaMod(tracksTexture, (uint8_t) chunk->value);
    SDL_RenderCopy(renderer, tracksTexture, NULL, NULL);    
    drawSpectrum(renderer, result, N/2+1);
    SDL_RenderPresent(renderer);
    
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
