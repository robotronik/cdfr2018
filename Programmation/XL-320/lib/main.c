#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "xl_320.h"

void test_fsm(){
  XL_320_Receiver_FSM fsm;
  XL_320_Init_Receiver_FSM(&fsm);
  while(1){
    unsigned int byte;
    scanf("%x", &byte);
    XL_320_Update_Receiver_FSM(&fsm, (uint8_t) byte);
    printf("%d\n", fsm.state);
  }
}

void test_build_frame(){
  uint8_t buffer[256];
  uint8_t params[9] = {0xFF, 0xFF, 0xFD, 0xFD, 0xFF, 0xFF, 0xFF, 0xFD, 0x87};
  XL_320_Instruction_Packet packet = {.id = 0x04, .instruction = PING, .nb_params = 9, .params = params};
  uint8_t size = XL_320_Build_Frame(&packet, buffer);
  uint8_t i;
  for(i=0; i < size; i++){
    printf("0x%2.2X ", buffer[i]);
  }
  printf("\n");
}

int main(){
  test_fsm();
  return EXIT_SUCCESS;
}
