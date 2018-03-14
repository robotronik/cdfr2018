#include "display.h"
#include "main.h"

#define BIT_K(n,k) ((n >> k) & 1)

void wait_refresh(void){
  while(!refresh);
  refresh = 0;
}

#define CAPAS_MGMT(gpio_state)						\
  {									\
    HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_Pin, gpio_state);		\
    HAL_GPIO_WritePin(BLANK_GPIO_Port, BLANK_Pin, gpio_state);		\
    HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, gpio_state);		\
  }									

#define PURGE_CAPAS CAPAS_MGMT(GPIO_PIN_SET);
#define REACTIVATE CAPAS_MGMT(GPIO_PIN_RESET);
#define SELECT_COL(num)							\
  {									\
    HAL_GPIO_WritePin(LINE_0_GPIO_Port, LINE_0_Pin, BIT_K((num),0));	\
    HAL_GPIO_WritePin(LINE_1_GPIO_Port, LINE_1_Pin, BIT_K((num),1));	\
    HAL_GPIO_WritePin(LINE_2_GPIO_Port, LINE_2_Pin, BIT_K((num),2));	\
    HAL_GPIO_WritePin(LINE_3_GPIO_Port, LINE_3_Pin, BIT_K((num),3));	\
    HAL_GPIO_WritePin(LINE_4_GPIO_Port, LINE_4_Pin, BIT_K((num),4));	\
  }

void write_row(uint32_t data){
  uint32_t mask = 1;

  int i = 0;
  for(; i < 10; i++){
    HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SERIAL_IN_GPIO_Port, SERIAL_IN_Pin, (data & mask)/mask);
    HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_SET);
    mask <<= 1; 
  }
}


void display_score(uint16_t score){
  int index[3] = {0, 0, 0};
  /*
   * Indices respectifs des trois chiffres indiquant le score (du poids
   * fort au poids faible). Ils sont mis à jour de manière asynchrone
   * lorsqu'un score est reçu sur la liaison UART.
   */

  //Getting score indexes
  __disable_irq();
  index[2] = score % 10; score /= 10;
  index[1] = score % 10; score /= 10;
  index[0] = score % 10;
  __enable_irq();

  int frame, row;

  for(frame = 0; frame < BRIGHTNESS_LEVELS-1; frame++){
    for(row = 0; row < (ROWS_PER_FRAME); row++){
      wait_refresh();

      //Purge capas
      PURGE_CAPAS;
      
            
      SELECT_COL(row+8);
      HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_RESET);

      uint32_t data = 0;
      uint8_t *number = sprites[index[0]][frame][row];
      data |= (number[0] << 2) | (number[1]>>6);
      data <<= 11;
      number = sprites[index[1]][frame][row];
      data |= (number[0] << 2) | (number[1]>>6);
      data <<= 11;
      number = sprites[index[2]][frame][row];
      data |= (number[0] << 2) | (number[1]>>6);

      int i;      
      for(i = 0; i < 32; i++){	
	HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERIAL_IN_GPIO_Port, SERIAL_IN_Pin, data&1);
	HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_SET);
	data >>= 1;
      }
     
      
      HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_SET);

      //Reactivate
      REACTIVATE;
    }
  }
  
}

void display_char(int index, int frame, int position){
  unsigned char (*frame_sprite)[BYTES_PER_ROW] = sprites[index][frame];

  //Pour chaque colonne
  int j;
  for(j = position; j < (ROWS_PER_FRAME + position); j++){
    //Attente du timer
    wait_refresh();
    
    //Purge des capas
    PURGE_CAPAS;

    //Sélection de la colonne
    SELECT_COL(j);

    //Sélection des leds
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_RESET);

    //Ici, il y a 8 lignes vides avant et après le caractère
    write_byte(0);
    
    int k;//Numéro d'octet
    for(k = 0; k < BYTES_PER_ROW; k++){
      write_byte(frame_sprite[j-position][k]);
    }

    write_byte(0);
    
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_SET);
    
    //Réactivation
    REACTIVATE;
  }
}

void blank_screen(){
  int i;
  for(i = 0; i < MATRIX_SIZE; i++){
    PURGE_CAPAS;
    SELECT_COL(i);
    //Sélection des leds
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_RESET);
    write_byte(0);
    write_byte(0);
    write_byte(0);
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_SET);
    REACTIVATE;
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
