#include "test_interface.h"

uint8_t send(uint8_t *data, uint16_t size, uint32_t timeout){
  int i;
  for(i = 0; i < size; i++){
    printf("0x%2.2X ", data[i]);
  }
  
  printf("\n");

  (void)timeout;
  return 0;
}

uint8_t receive(uint8_t *data, uint16_t size, uint32_t timeout){
  while(size){
    scanf("%02hhx", data++);
    size--;
  }

  (void)timeout;
  return 0;
}
