#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "xl_320.h"

int main(){
  uint8_t buffer[256];
  uint8_t params[9] = {0xFF, 0xFF, 0xFD, 0xFD, 0xFF, 0xFF, 0xFF, 0xFD, 0x87};
  XL_320_Instruction_Packet packet = {.id = 0x04, .instruction = PING, .nb_params = 9, .params = params};
  uint8_t size = build_frame(&packet, buffer);
  uint8_t i;
  for(i=0; i < size; i++){
    printf("0x%2.2X ", buffer[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}
