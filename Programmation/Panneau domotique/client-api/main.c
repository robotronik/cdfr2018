#include "main.h"

int main(){
  uint16_t *score = SC_Start("192.168.0.3", "80");
  sleep(5);
  *score = 2;
  SC_Update();
  sleep(5);
  SC_Stop();
  return EXIT_SUCCESS;
}

