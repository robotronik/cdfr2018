#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "ax_12a.h"

//printf("0x%2.2X");
//scanf("%x",&byte);

void set_direction(){}

uint32_t get_tick(){
  return 0;
}

uint8_t receive(uint8_t *data, uint16_t size, uint32_t timeout){
  while(size){
    scanf("%x", data++);
    size--;
  }
  return 0;
}

uint8_t send(uint8_t *data, uint16_t length, uint32_t timeout){
  int i;
  for(i = 0; i < length; i++){
    printf("0x%2.2X ", data[i]);
  }
  return 0;
}

int main(){
  AX_Interface interface;
  interface.receive = receive;
  interface.send = send;
  interface.set_direction = set_direction;
  
  /*if(AX_Receive(&interface, 9, 1)){
    printf("Erreur");
    return EXIT_SUCCESS;
  }

  printf("\n");
  printf("ID : 0x%2.2X\n", interface.status.id);
  printf("Err : 0x%2.2X\n", interface.status.err);
  printf("Nb_params : 0x%2.2X\n", interface.status.nb_params);
  printf("params : ");
  int i = 0;
  for(i = 0; i < interface.status.nb_params; i++){
    printf("0x%2.2X ", interface.status.params[i]);
  }
  printf("\n");*/

  uint8_t params[3] = {0x0C, 0x64, 0xAA};
  AX_Instruction_Packet packet = {.id = 0x01, .instruction = AX_WRITE, .nb_params = 3, .params = params};
  AX_Send(&interface, &packet, 1);
  
  return EXIT_SUCCESS;
}
