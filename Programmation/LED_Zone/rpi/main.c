#include "song.h"
#include <SDL2/SDL.h>
#include <fftw3.h>
#include <wiringPi.h>
#include <softPwm.h>

#define CHUNK_PERIOD 50
#define FREQUENCY_STEP (1000 / CHUNK_PERIOD)
#define BUFFER_SIZE_MS 25
#define DEBUG 1

int main(int argc, char *argv[]){
  if(argc < 2){
    return EXIT_FAILURE;
  }

  //WiringPi
  if(wiringPiSetup() == -1){
    return EXIT_FAILURE;
  }
  
  softPwmCreate(12, 0, 255);
  
  //Song
  Song *song;
  Chunk *chunk;
  
  if(SDL_Init(SDL_INIT_AUDIO) != 0){
    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  
  song = Load_Song(argv[1], BUFFER_SIZE_MS);
  if(song == NULL){
    return EXIT_FAILURE;
  }
  
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

  int go = 1;
  double max = 0.;
  while(Is_Playing_Song(song) && go){    
#if 0
    uint32_t time = Get_Time_Song(song);
    printf("Time : %d\n", time);
#endif

    if(Update_Chunk(chunk)){
      //Show current chunk value

      softPwmWrite(12, (int) (pow(chunk->value/255, 3)*255.));
      
      #if DEBUG
      int j;
      printf("\r");
      for(j = 0; j < 25; j++){
	if(j*10. < chunk->value){
	  printf("#");
	}else{
	  printf("-");
	}
      }
      #endif

      //printf("POM %lf\n", chunk->value);
      fflush(stdout);
      
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
  SDL_Quit();
  return EXIT_SUCCESS;
}
