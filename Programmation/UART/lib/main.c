#include "test_interface.h"
#include "rpv1.h"
#include "rc_server.h"

void test_crc();

void RP_Packet_Received(RP_Interface *iface, RP_Packet *packet){
  printf("\nPacket received.\nSize : %d bytes\nData :\n", packet->len);

  int i;
  for(i = 0; i < packet->len; i++){
    printf("0x%2.2X ", packet->data[i]);
  }
  printf("\n\n");
}

void RP_Error_Handler(RP_Interface *iface, uint16_t error){
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

void go(uint8_t id, uint8_t *data, uint8_t len){
  
}
int RC_Pack_Vars(RC_Server *server, int id, uint8_t *out, int out_len,  ...);

int main(){
  RC_Server server;

  RC_Server_Init(&server);
  int r = RC_Server_Add_Function(&server, 0, go, "ifFs", "Fs", RC_IMMEDIATE);
  printf("%d\n", r);

  uint8_t buff[256];
  r = RC_Pack_Vars(&server, 0, buff, 256, 5, -6.23, 3.14, "pack string motherfucker");//Str 25
  
  printf("%d\n", r);
  printf("%d\n", *((int*) (buff)));
  printf("%f\n", *((float*) (buff+sizeof(int))));
  printf("%f\n", *((double*) (buff+sizeof(int)+sizeof(float))));
  printf("%s\n", ((char*) (buff+sizeof(int)+sizeof(float)+sizeof(double))));
  printf("%d %d %d\n", sizeof(int), sizeof(float), sizeof(double));
  
  
  return 0;
  
  uint16_t size = 5;
  RP_Packet packet = {
    .len = size,
  };

  RP_Interface interface;
  RP_Init_Interface(&interface, send);
  
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
  }

  RP_Process_Data(&interface, interface.buffer_in, RP_BUFFER_SIZE);
 
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
