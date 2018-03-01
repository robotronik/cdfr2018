#include "main.h"

int main(){
  SC_Start("2", "192.168.0.3", "80");
  uint16_t score;
  int i;
  for(i=0; i < 100; i++){
    score = i;
    SC_Update(score);
    sleep(1);
  }
  sleep(120);
  SC_Stop();
  return EXIT_SUCCESS;
}

