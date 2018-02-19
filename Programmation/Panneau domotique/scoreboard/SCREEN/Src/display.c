#include "display.h"
#include "main.h"

#define BIT_K(n,k) ((n >> k) & 1)

void wait_refresh(void){
  while(!refresh);
  refresh = 0;
}

void display_char(int index, int frame, int position){
  unsigned char (*frame_sprite)[BYTES_PER_ROW] = sprites[index][frame];

  //Pour chaque colonne
  int j;
  for(j = position; j < (ROWS_PER_FRAME + position); j++){
    //Attente du timer
    wait_refresh();
    
    //Purge des capas
    HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BLANK_GPIO_Port, BLANK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, GPIO_PIN_SET);

    //Sélection de la colonne
    HAL_GPIO_WritePin(LINE_0_GPIO_Port, LINE_0_Pin, BIT_K(j,0));
    HAL_GPIO_WritePin(LINE_1_GPIO_Port, LINE_1_Pin, BIT_K(j,1));
    HAL_GPIO_WritePin(LINE_2_GPIO_Port, LINE_2_Pin, BIT_K(j,2));
    HAL_GPIO_WritePin(LINE_3_GPIO_Port, LINE_3_Pin, BIT_K(j,3));
    HAL_GPIO_WritePin(LINE_4_GPIO_Port, LINE_4_Pin, BIT_K(j,4));

    //Sélection des leds
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_RESET);

    //Ici, il y a 8 lignes vides avant et après le caractère
    write_byte(0);
    
    int k;//Numéro d'octet
    for(k = 0; k < BYTES_PER_ROW; k++){
      write_byte(frame_sprite[j][k]);
    }

    write_byte(0);
    
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_SET);
    
    //Réactivation
    HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BLANK_GPIO_Port, BLANK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_Pin, GPIO_PIN_RESET);
  }
}

void write_byte(unsigned char byte){
  unsigned char mask = 0b10000000;//128 = 2^7
  
  while(mask){
    HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SERIAL_IN_GPIO_Port, SERIAL_IN_Pin, (byte & mask)/mask);
    HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_SET);
    mask >>= 1;
  }
}
