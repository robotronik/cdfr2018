#include "pseudo_main.h"

int main(){
  uint8_t index[NB_DIGITS];
  get_sprite_index(123, index);
  printf("%d %d %d\n", index[0], index[1], index[2]);
  
  uint8_t frame;
  while(1){
    //Update score if neede
    
    for(frame = 0; frame < BRIGHTNESS_LEVELS-1; frame++){
	print_frame(frame, index[0], 0);
	blank_row(10);
	print_frame(frame, index[1], 11);
	blank_row(21);
	print_frame(frame, index[2], 22);
    }
  }
  
  return 0;
}

void print_frame(uint8_t frame, uint8_t index, uint8_t offset){
  int row;
  int spacing = (MATRIX_SIZE-ROW_LENGTH)/2;
  for(row = offset; row < offset + ROWS_PER_FRAME; row++){
    unsigned char *row_sprite = sprites[index][frame][row];
    int led;
    for(led = 0; led < spacing; led++){
      //Set led off
    }
    uint8_t mask = 128;
    for(; led < spacing+8; led++){
      //Set led on if (*row_sprite)&mask != 0
      mask /= 2;
    }
    row_sprite++;
    mask = 128;
    for(; led < spacing+16; led++){
      //Set led on if (*row_sprite)&mask != 0
      mask /= 2;
    }
    for(; led < MATRIX_SIZE; led++){
      //Set led off
    }
  }
}

void blank_row(uint8_t row){
  
}

void get_sprite_index(uint16_t score, uint8_t index[NB_DIGITS]){
  if(score > MAX_SCORE){
    score = MAX_SCORE;
  }

  int i;
  for(i = NB_DIGITS-1; i >= 0; i--){
    index[i] = score%10;
    score /= 10;
  }
}

