#include "song.h"
#include "main.h"

#include <SDL2/SDL.h>
#include <fftw3.h>
#include <wiringPi.h>
#include <softPwm.h>

static uint32_t Player_Is_Playing(Player *player);
static uint32_t Player_Get_Time(Player *player);

Player* Init_Player(const char* file, uint32_t dt){
  Player *player = malloc(sizeof(*player));
  if(player == NULL){
    return NULL;
  }

  if(SDL_LoadWAV(file, &player->wav_spec, &player->wav_buffer, &player->wav_len) == NULL){
    fprintf(stderr, "SDL_LoadWAV error: %s\n", SDL_GetError());
    free(player);
    return NULL;
  }

  player->wav_spec.callback = NULL;
  player->wav_spec.samples = player->wav_spec.freq / (1000/dt);
  //printf("%d\n", player->wav_spec.samples);
  player->dev = SDL_OpenAudioDevice(NULL, 0, &player->wav_spec, &player->dev_spec, 0);
  /*
   * Open default device (NULL) for playback (0). The last '0' tells
   * the SDL to convert original format to effective format if needed.
   */
  if(player->dev == 0){
    fprintf(stderr, "SDL_OpenAudioDevice error: %s\n", SDL_GetError());
    SDL_CloseAudioDevice(player->dev);
    free(player);
    return NULL;
  }
  
  return player;
}

void Destroy_Player(Player **player){
  if(player == NULL || *player == NULL){
    return;
  }
  
  SDL_CloseAudioDevice((*player)->dev);
  SDL_FreeWAV((*player)->wav_buffer);
  free(*player);
  *player = NULL;
}

void Play(Player *player, int pwmPin){
  if(player == NULL){
    return;
  }

  //Queue the whole song
  SDL_QueueAudio(player->dev, player->wav_buffer, player->wav_len);

  //Start playing
  SDL_PauseAudioDevice(player->dev, 0);

  //Init sync variables
  player->pos = 0;
  player->queued = player->wav_len;
  player->last_queued = player->queued;

  Chunk *chunk;
  chunk = Make_Chunk(player, CHUNK_PERIOD);
  if(chunk == NULL){
    return;
  }

  //FFT
  double *in, *result;
  fftw_complex *out;
  fftw_plan p;
  int N = (CHUNK_PERIOD * player->wav_spec.freq)/1000;
  in = (double*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2 + 1));
  p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
  result = malloc(sizeof(*result)*(N/2+1));

  int go = 1;
  double max = 0.;
  while(Player_Is_Playing(player) && go){    
#if 0
    uint32_t time = Player_Get_Time(player);
    printf("Time : %d\n", time);
#endif

    if(Update_Chunk(chunk)){
      //Show current chunk value
      uint8_t level = ((chunk->value-100.)/155.) * 255;
      if(pwmPin != -1){
	softPwmWrite(pwmPin, (chunk->value > 100.)?level:0);
      }
      
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
      fflush(stdout);
#endif
      
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

  Free_Chunk(&chunk);
  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
}


static uint32_t Player_Get_Time(Player *player){
  if(player == NULL){
    return -1;
  }

  player->last_queued = player->queued;
  player->queued = SDL_GetQueuedAudioSize(player->dev);
  player->pos += player->last_queued - player->queued;
  
  
  return (uint32_t) (1000 * ((player->pos) / (2.*player->dev_spec.freq)));
}



static uint32_t Player_Is_Playing(Player *player){
  Player_Get_Time(player);
  return player->pos < player->wav_len;
}

Chunk* Make_Chunk(Player *player, uint32_t period){
  Chunk* chunk = malloc(sizeof(*chunk));

  if(chunk == NULL){
    return NULL;
  }

  chunk->date = 0;
  chunk->period = period;
  chunk->len = (period * player->wav_spec.freq)/1000;
  chunk->value = 0;
  chunk->start = (int16_t*) player->wav_buffer;
  chunk->player = player;
  
  return chunk;
}

uint32_t Update_Chunk(Chunk *chunk){
  uint32_t time = Player_Get_Time(chunk->player);

  if(time >= chunk->date ){
    chunk->date += chunk->period;
    chunk->start += chunk->len;
  }
  else{
    return 0;
  }

  //Check that we don't overflow
  if((((uint8_t*) chunk->start + 2*chunk->len) - chunk->player->wav_buffer) >= (int) chunk->player->wav_len){
    return 0;
  }

  return 1;
}

void Free_Chunk(Chunk **chunk){
  if(chunk == NULL || *chunk == NULL){
    return;
  }
  free(*chunk);
  *chunk = NULL;
}
