#ifndef SONG_H
#define SONG_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct Song_S{
  //Audio device
  SDL_AudioDeviceID dev;
  SDL_AudioSpec dev_spec;

  //WAV
  SDL_AudioSpec wav_spec;
  uint8_t *wav_buffer;
  uint32_t wav_len;

  //Sync
  uint32_t pos;
  uint32_t queued;
  uint32_t last_queued;
}Song;

Song* Load_Song(const char* file, uint32_t dt);
void Play_Song(Song *song);
uint32_t Get_Time_Song(Song *song);
uint32_t Is_Playing_Song(Song *song);
void Free_Song(Song **song);

typedef struct Chunk_S{
  uint32_t date;
  uint32_t period;//ms
  uint32_t len;//samples
  double value;
  int16_t *start;
  Song *song;
}Chunk;

Chunk* Make_Chunk(Song *song, uint32_t period);
uint32_t Update_Chunk(Chunk *chunk);
void Free_Chunk(Chunk **chunk);

#endif
