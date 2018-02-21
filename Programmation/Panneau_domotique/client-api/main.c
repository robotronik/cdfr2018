#include "main.h"

int main(){
  uint16_t *score = SC_Start("2", "192.168.0.3", "80");
  int i;
  for(i=0; i < 100; i++){
    *score = i;
    SC_Update();
    sleep(1);
  }
  sleep(120);
  SC_Stop();
  return EXIT_SUCCESS;
}

