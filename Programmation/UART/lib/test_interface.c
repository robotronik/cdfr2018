#include "test_interface.h"

uint8_t send1(uint8_t *data, uint16_t size, uint32_t timeout){
  send(data, size, &interface2);
  return 0*timeout;
}

uint8_t send2(uint8_t *data, uint16_t size, uint32_t timeout){
  send(data, size, &interface1);
  return 0*timeout;
}

uint8_t send(uint8_t *data, uint16_t size, RP_Interface *target){
  /*int i;
  for(i = 0; i < size; i++){
    printf("0x%2.2X ", data[i]);
  }
  
  printf("\n");*/

  RP_Process_Data(target, data, size);  

  return 0;
}


void RP_Packet_Received(RP_Interface *iface, RP_Packet *packet){

  printf("\n\n");
  if(iface == &interface1){
    printf("Serveur : \n");
  }else{
    printf("Client: \n");
  }
  
  printf("Packet received.\nID : %d\nSize : %d bytes\nData :\n", packet->id, packet->len);

  int i;
  for(i = 0; i < packet->len; i++){
    printf("0x%2.2X ", packet->data[i]);
  }
  printf("\n\n");

  if(iface == &interface1){
    RC_Server_Get_Request(&server, packet);
  }
}

void RP_Error_Handler(RP_Interface *iface, uint16_t error){
  (void)iface;
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

#if 0
uint8_t receive(uint8_t *data, uint16_t size, uint32_t timeout, RP_Interface *iface){
  while(size){
    scanf("%02hhx", data++);
    size--;
  }

  (void)timeout;
  return 0;
}
#endif
