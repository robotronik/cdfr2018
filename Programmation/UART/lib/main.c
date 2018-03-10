#include "test_interface.h"
#include "rpv1.h"
#include "rc_server.h"

void test_crc();

RP_Interface interface1;
RP_Interface interface2;

int RC_Pack_Vars_Test(const char *fmt, uint8_t *out, int out_len,  ...);

RC_Server server;
void go(int id, uint8_t *data, int len){
  int a; float b; double c; char string[RC_STR_SIZE];
  RC_Server_Get_Args(&server, id, data, len, &a, &b, &c, string);
  
  printf("%d\n%f\n%lf\n%s", a, b, c, string);

  RC_Server_Return(&server, id, c+1, "success");
}

int main(){

  RP_Init_Interface(&interface1, send1);
  RP_Init_Interface(&interface2, send2);

  RC_Server_Init(&server, &interface1);

  RC_Client client;
  RC_Client_Init(&client, &interface2);
  
  RC_Client_Add_Function(&client, 0, go, "ifFs", "Fs");
  RC_Server_Add_Function(&server, 0, go, "ifFs", "Fs", RC_IMMEDIATE);

  double r;
  char str[RC_STR_SIZE];
  RC_Call(&client, 0, 5, -6.23, 3.141592, "pack string motherfucker !", &r, str);

  printf("\n%lf\n%s\n", r, str);
  
  return 0;
  
  #if 0

  
  uint16_t size = 5;
  RP_Packet packet = {
    .len = size,
  };
  
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
  #endif
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
