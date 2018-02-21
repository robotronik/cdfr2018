#include "test_interface.h"
#include "rpv1.h"

void test_crc();

int main(){  
  uint16_t size = 5;
  RP_Packet packet = {
    .len = size,
  };

  RP_Interface interface;
  RP_Init_Interface(&interface, send, receive);
  
  //uint32_t pos[2] = {1682, 3578};

  interface.receive(packet.data, size, 1);
  interface.send(packet.data, size, 1);
  
  RP_Send(&interface, &packet, 1);

  //Erreur
  //interface.buffer[2] = 2;
  
  int i;
  for(i = 0; i < RP_BUFFER_SIZE && !interface.fsm.valid && !RP_Get_Error(); i++){
    interface.fsm.in = interface.buffer + i;
    interface.fsm.update_state(&interface.fsm);
  }
  packet.len = interface.fsm.size;

  if(interface.fsm.valid){
    for(i = 0; i < packet.len; i++){
      printf("0x%2.2X ", packet.data[i]);
    }
  }else{

    switch(RP_Get_Error()){
    case (RP_ERR_LINK | RP_ERR_UNEXPECTED_EOF):
      printf("Unexpected EOF\n");
      break;
    case (RP_ERR_LINK | RP_ERR_SIZE):
      printf("Bad size\n");
      break;
    case (RP_ERR_LINK | RP_ERR_CRC):
      printf("Bad CRC\n");
      break;
    }
  }
  return 0;
}

void test_crc(){
  uint16_t crc = CRC_INIT;
  UPDATE_CRC(crc,0xFF);
  UPDATE_CRC(crc,0xFF);
  UPDATE_CRC(crc,0xFD);
  UPDATE_CRC(crc,0x00);
  UPDATE_CRC(crc,0x01);
  UPDATE_CRC(crc,0x03);
  UPDATE_CRC(crc,0x00);
  UPDATE_CRC(crc,0x01);
  printf("CRC : 0x%2.2X 0x%2.2X\n", crc&0xFF, crc>>8);
  //http://crccalc.com/ 0x4E19 CRC-16 buypass
}
