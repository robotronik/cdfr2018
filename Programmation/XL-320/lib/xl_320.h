/*
 * Librairie XL-320 @Robotronik
 * xl_320.h
 * Ecrit pour un µC en little-endian
 * Il est autorisé de rager librement avec ce fichier.
 */
#ifndef XL_320_HEADER
#define XL_320_HEADER

#include <stdint.h>

#define XL_320_BUFFER_SIZE 256

typedef struct XL_320_Interface_S{
  void *(send)(uint8_t *, uint16_t, uint32_t);//data, size, timeout ms
  void *(receive)(uint8_t *, uint16_t, uint32_t);//data, size, timeout ms
}XL_320_Interface;

typedef struct XL_320_S{
  uint8_t id;
  XL_320_Interface interface;
}XL_320;

typedef enum XL_320_Instruction_E{
  PING = 0x01,
  READ = 0x02,
  WRITE = 0x03,
  REG_WRITE = 0x04,
  ACTION = 0x05,
  FACTORY_RESET = 0x06,
  REBOOT = 0x08,
  STATUS = 0x55,
  SYNC_READ = 0x82,
  SYNC_WRITE = 0x83,
  BULK_READ = 0x92,
  BULK_WRITE = 0x93
}XL_320_Instruction;

typedef enum XL_320_Error_E{
  ERR_ILLEGAL_ARGUMENTS,
  ERR_BUFFER_OVERFLOW,
  ERR_ILLEGAL_ID,
  ERR_BAD_FRAME,
}XL_320_Error;

typedef struct XL_320_Instruction_Packet_S{
  uint8_t id;
  XL_320_Instruction instruction;
  uint8_t nb_params;
  uint8_t *params;
}XL_320_Instruction_Packet;

typedef enum XL_320_Status_Error_E{
  FAIL = 0x01,
  BAD_INSTRUCTION = 0x02,
  CRC_ERROR = 0x03,
  DATA_RANGE_ERROR = 0x04,
  DATA_LENGTH_ERROR = 0x05,
  DATA_LIMIT_ERROR = 0x06,
  ACCESS_ERROR = 0x07
}XL_320_Status_Error;

#define STATUS_ALERT(error_byte) (error_byte>>7)
#define STATUS_ERROR(error_byte) (error_byte&0b0111111)

typedef struct XL_320_Status_Packet_S{
  uint8_t id;
  uint8_t err;
  uint8_t nb_params;
  uint8_t params[XL_320_BUFFER_SIZE];
}XL_320_Status_Packet;

//Réception d'un paquet
typedef enum XL_320_Receiver_FSM_State_E{
  WAITING_FOR_HEADER_0,
  WAITING_FOR_HEADER_1,
  WAITING_FOR_HEADER_2,
  WAITING_FOR_HEADER_3,
  PACKET_ID_BYTE,
  PACKET_LENGTH_LOW,
  PACKET_LENGTH_HIGH,
  INSTRUCTION_BYTE,
  RECEIVING_PACKET,
  ERROR,
  SUCCESS  
}XL_320_Receiver_FSM_State;

typedef struct XL_320_Receiver_FSM_S{
  XL_320_Receiver_FSM_State state;
  uint16_t length;
  uint8_t buffer[XL_320_BUFFER_SIZE];
  uint8_t *p_buffer;
}XL_320_Receiver_FSM;

void XL_320_Init_Receiver_FSM(XL_320_Receiver_FSM *fsm);
void XL_320_Update_Receiver_FSM(XL_320_Receiver_FSM *fsm, uint8_t byte);
uint8_t XL_320_Extract_Status_Packet(XL_320_Status_Packet *packet, uint8_t frame[XL_320_BUFFER_SIZE], uint16_t length);
uint8_t XL_320_Receive(XL_320_Status_Packet *packet, uint32_t timeout);

//Envoi d'un paquet
uint8_t XL_320_Build_Frame(XL_320_Instruction_Packet *packet, uint8_t buffer[XL_320_BUFFER_SIZE]);
/*
 * Construit une trame à partir d'un paquet instruction et l'écrit dans le buffer.
 * La fonction retourne la taille effective de la trame construite en cas de succès, 0 en cas d'erreur.
 */

//Contrôle de redondance cyclique
uint16_t XL_320_Update_CRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

#endif
