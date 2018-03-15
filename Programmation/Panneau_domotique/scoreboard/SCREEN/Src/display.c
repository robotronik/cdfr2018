#include "display.h"
#include "main.h"

#define BIT_K(n,k) ((n >> k) & 1)

#define CAPAS_MGMT(gpio_state)						\
  {									\
    HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_Pin, gpio_state);		\
    HAL_GPIO_WritePin(BLANK_GPIO_Port, BLANK_Pin, gpio_state);		\
    HAL_GPIO_WritePin(PURGE_GPIO_Port, PURGE_Pin, gpio_state);		\
  }									

#define PURGE_CAPAS CAPAS_MGMT(GPIO_PIN_SET)
#define REACTIVATE CAPAS_MGMT(GPIO_PIN_RESET)
#define SELECT_COL(num)							\
  {									\
    HAL_GPIO_WritePin(LINE_0_GPIO_Port, LINE_0_Pin, BIT_K((num),0));	\
    HAL_GPIO_WritePin(LINE_1_GPIO_Port, LINE_1_Pin, BIT_K((num),1));	\
    HAL_GPIO_WritePin(LINE_2_GPIO_Port, LINE_2_Pin, BIT_K((num),2));	\
    HAL_GPIO_WritePin(LINE_3_GPIO_Port, LINE_3_Pin, BIT_K((num),3));	\
    HAL_GPIO_WritePin(LINE_4_GPIO_Port, LINE_4_Pin, BIT_K((num),4));	\
  }

#define WRITE_BIT(bit)							\
  {									\
    HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_RESET);	\
    HAL_GPIO_WritePin(SERIAL_IN_GPIO_Port, SERIAL_IN_Pin, bit);		\
    HAL_GPIO_WritePin(CLK_COL_GPIO_Port, CLK_COL_Pin, GPIO_PIN_SET);	\
  }

void wait_refresh(void){
  while(!refresh);
  refresh = 0;
}

void blank_screen(){
  int i;
  for(i = MATRIX_SIZE; i > 0; i--){
    PURGE_CAPAS;
    SELECT_COL(i);
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_RESET);
    int j;
    for(j = MATRIX_SIZE; j > 0; j--){
      WRITE_BIT(0);
    }
    HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_SET);
    REACTIVATE;
  }
}

void display_score(uint16_t score){

  //Indexs du score
  __disable_irq();
  uint8_t ***const sprite_k[3] = {(uint8_t***) sprites[score%10],
				(uint8_t***) sprites[(score /= 10)%10],
				(uint8_t***) sprites[(score /= 10)%10]};
			
  __enable_irq();

  int frame, row;
  for(frame = 0; frame < BRIGHTNESS_LEVELS-1; frame++){
    uint8_t **const frame_k[3] = {(uint8_t**) sprite_k[0][frame],
				 (uint8_t**) sprite_k[1][frame],
				 (uint8_t**) sprite_k[2][frame]};
    
    for(row = 0; row < ROWS_PER_FRAME; row++){
      uint8_t *const row_k[3] = {(uint8_t*) frame_k[0][row],
				 (uint8_t*) frame_k[1][row],
				 (uint8_t*) frame_k[2][row]};
      
      uint32_t data =
	((uint32_t) ((((uint16_t) row_k[2][0]) << 8) | row_k[2][1]) << 16) |
        ((uint32_t) ((((uint16_t) row_k[1][0]) << 8) | row_k[1][1]) << 5) |
        ((uint32_t) ((((uint16_t) row_k[0][0]) << 8) | row_k[0][1]) >> 6);
      
      //Attente du timer
      wait_refresh();

      //Purge capas
      PURGE_CAPAS;
      
      //Sélection de la colonne
      SELECT_COL(8 + row);

      //Sélection des leds
      HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_RESET);      

      int i;
      const uint32_t mask = 1 << 31;
      for(i = 32; i > 0; i--){
	WRITE_BIT(((data&mask) >> 31));
	data <<= 1;
      }
      
      HAL_GPIO_WritePin(LATCH_COL_GPIO_Port, LATCH_COL_Pin, GPIO_PIN_SET);

      //Réactivation
      REACTIVATE;
    }
  }
  
}



