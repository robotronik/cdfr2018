#include "test_interface.h"
#include "rpv1.h"

void test_crc();

void RP_Packet_Received(RP_Packet *packet){
  printf("\nPacket received.\nSize : %d bytes\nData :\n", packet->len);

  int i;
  for(i = 0; i < packet->len; i++){
    printf("0x%2.2X ", packet->data[i]);
  }
  printf("\n\n");
}

void RP_Error_Handler(uint16_t error){
  switch(RP_ERROR_TYPE(error)){
  case RP_ERR_INTERNAL:
    printf("Internal error: ");
    switch(RP_INTERNAL_ERROR(error)){
    case RP_ERR_ILLEGAL_ARGUMENTS:
      printf("Illegal arguments\n");
      break;
    case RP_ERR_BUFFER_OVERFLOW:
      printf("Buffer overflow\n");
      break;
    default:
      printf("Unknown\n");
      break;
    }
    break;
  case RP_ERR_LINK:
    printf("Link error: ");
    switch(RP_LINK_ERROR(error)){
    case RP_ERR_TIMEOUT:
      printf("Timeout\n");
      break;
    case RP_ERR_UNEXPECTED_EOF:
      printf("Unexpected EOF\n");
      break;
    case RP_ERR_SIZE:
      printf("Size error\n");
      break;
    case RP_ERR_CRC:
      printf("CRC Error\n");
      break;
    default:
      printf("Unknown\n");
      break;
    }
    break;
  default:
    printf("Unknown error\n");
    break;
  }
}

int main(){  
  uint16_t size = 5;
  RP_Packet packet = {
    .len = size,
  };

  RP_Interface interface;
  RP_Init_Interface(&interface, send, RP_Packet_Received, RP_Error_Handler);
  
  receive(packet.data, 5, 1);
  
  interface.send(packet.data, size, 1);
  
  RP_Send(&interface, &packet, 1);

  int i;
  
  for(i = 0; i < RP_BUFFER_SIZE; i++){
    interface.buffer_in[i] = interface.buffer_out[i];

    //Erreur
#if 0
    if(i == 2){
      interface.buffer_in[i] = 2;
    }
#endif
    
    interface.fsm.update_state(&interface.fsm);
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
