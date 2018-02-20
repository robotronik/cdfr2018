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

  int i;
  
  for(i = 0; i < RP_BUFFER_SIZE && !interface.fsm.valid; i++){
    interface.fsm.in = interface.buffer + i;
    interface.fsm.update_state(&interface.fsm);
  }
  packet.len = interface.fsm.size;
  
  for(i = 0; i < packet.len; i++){
    printf("0x%2.2X ", packet.data[i]);
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
