#ifndef SONG_H
#define SONG_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct Player_S{
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
}Player;

Player* Init_Player(const char* file, uint32_t dt);
/*
 * Load a WAV from 'file' path and create a player to play it.
 * Returns a pointer to an initialized Player struct on success, NULL
 * otherwise.
 */

void Destroy_Player(Player **player);
/*
 * Free the player '**player' and set its pointer to NULL.
 */

void Play(Player *player, int pwmPin);
/*
 * Play the song loaded by Init_Player and control the pwmPin for
 * light effect. If pwmPin == -1, the light effect is disabled.
 */


typedef struct Chunk_S{
  uint32_t date;
  uint32_t period;//ms
  uint32_t len;//samples
  double value;
  int16_t *start;
  Player *player;
}Chunk;

Chunk* Make_Chunk(Player *player, uint32_t period);
uint32_t Update_Chunk(Chunk *chunk);
void Free_Chunk(Chunk **chunk);

#endif
