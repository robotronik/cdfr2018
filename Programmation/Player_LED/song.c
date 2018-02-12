#include "song.h"

Song* Load_Song(const char* file, uint32_t dt){
  Song *song = malloc(sizeof(*song));
  if(song == NULL){
    return NULL;
  }

  if(SDL_LoadWAV(file, &song->wav_spec, &song->wav_buffer, &song->wav_len) == NULL){
    fprintf(stderr, "SDL_LoadWAV error: %s\n", SDL_GetError());
    free(song);
    return NULL;
  }

  song->wav_spec.callback = NULL;
  song->wav_spec.samples = song->wav_spec.freq / (1000/dt);
  //printf("%d\n", song->wav_spec.samples);
  song->dev = SDL_OpenAudioDevice(NULL, 0, &song->wav_spec, &song->dev_spec, 0);
  /*
   * Open default device (NULL) for playback (0). The last '0' tells
   * the SDL to convert original format to effective format if needed.
   */
  if(song->dev == 0){
    fprintf(stderr, "SDL_OpenAudioDevice error: %s\n", SDL_GetError());
    SDL_CloseAudioDevice(song->dev);
    free(song);
    return NULL;
  }
  
  return song;
}

void Play_Song(Song *song){
  if(song == NULL){
    return;
  }

  //Queue the whole song
  SDL_QueueAudio(song->dev, song->wav_buffer, song->wav_len);

  //Start playing
  SDL_PauseAudioDevice(song->dev, 0);

  //Init sync variables
  song->pos = 0;
  song->queued = song->wav_len;
  song->last_queued = song->queued;
}

uint32_t Get_Time_Song(Song *song){
  if(song == NULL){
    return -1;
  }

  song->last_queued = song->queued;
  song->queued = SDL_GetQueuedAudioSize(song->dev);
  song->pos += song->last_queued - song->queued;
  
  
  return (uint32_t) (1000 * ((song->pos) / (2.*song->dev_spec.freq)));
}

void Free_Song(Song **song){
  if(song == NULL || *song == NULL){
    return;
  }
  
  SDL_CloseAudioDevice((*song)->dev);
  SDL_FreeWAV((*song)->wav_buffer);
  free(*song);
  *song = NULL;
}

uint32_t Is_Playing_Song(Song *song){
  Get_Time_Song(song);
  return song->pos < song->wav_len;
}

Chunk* Make_Chunk(Song *song, uint32_t period){
  Chunk* chunk = malloc(sizeof(*chunk));

  if(chunk == NULL){
    return NULL;
  }

  chunk->date = 0;
  chunk->period = period;
  chunk->len = (period * song->wav_spec.freq)/1000;
  chunk->value = 0;
  chunk->start = (uint16_t*) song->wav_buffer;
  chunk->song = song;
  
  return chunk;
}

uint32_t Update_Chunk(Chunk *chunk){
  uint32_t time = Get_Time_Song(chunk->song);
  
  if(time >= chunk->date){
    chunk->date += chunk->period;
    chunk->start += chunk->len;
  }
  else{
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
