/*
 * Librairie XL-320 @Robotronik
 * xl_320.h
 * Ecrit pour un µC en little-endian
 * Il est autorisé de rager librement avec ce fichier.
 */
#ifndef XL_320_HEADER
#define XL_320_HEADER

#include <stdint.h>

#define XL_BUFFER_SIZE 256

/*
 * Codes d'instructions du XL_320
 */
typedef enum XL_Instruction_E{
  XL_PING = 0x01,
  XL_READ = 0x02,
  XL_WRITE = 0x03,
  XL_REG_WRITE = 0x04,
  XL_ACTION = 0x05,
  XL_FACTORY_RESET = 0x06,
  XL_REBOOT = 0x08,
  XL_STATUS = 0x55,
  XL_SYNC_READ = 0x82,
  XL_SYNC_WRITE = 0x83,
  XL_BULK_READ = 0x92,
  XL_BULK_WRITE = 0x93
}XL_Instruction;

/*
 * Codes d'erreur internes à la librairie
 */
typedef enum XL_Error_E{
  XL_ERR_ILLEGAL_ARGUMENTS,
  XL_ERR_BUFFER_OVERFLOW,
  XL_ERR_ILLEGAL_ID,
  XL_ERR_BAD_FRAME,
  XL_ERR_TIMEOUT,
}XL_Error;

/*
 * Codes d'erreurs dans les paquets "status"
 */
#define STATUS_ALERT(error_byte) (error_byte>>7)
#define STATUS_ERROR(error_byte) (error_byte&0b0111111)
typedef enum XL_Status_Error_E{
  XL_FAIL = 0x01,
  XL_BAD_INSTRUCTION = 0x02,
  XL_CRC_ERROR = 0x03,
  XL_DATA_RANGE_ERROR = 0x04,
  XL_DATA_LENGTH_ERROR = 0x05,
  XL_DATA_LIMIT_ERROR = 0x06,
  XL_ACCESS_ERROR = 0x07
}XL_Status_Error;

/*
 * Etats de la FSM gérant la réception d'une trame
 */
typedef enum XL_Receiver_FSM_State_E{
  XL_WFH_0,//Waiting for header
  XL_WFH_1,
  XL_WFH_2,
  XL_WFH_3,
  XL_PACKET_ID_BYTE,
  XL_PACKET_LENGTH_LOW,
  XL_PACKET_LENGTH_HIGH,
  XL_INSTRUCTION_BYTE,
  XL_RECEIVING_PACKET,
  XL_FSM_ERROR,
  XL_FSM_SUCCESS  
}XL_Receiver_FSM_State;

typedef struct XL_Interface_S{
  void (*send)(uint8_t *, uint16_t, uint32_t);//data, size, timeout ms
  void (*receive)(uint8_t *, uint16_t, uint32_t);//data, size, timeout ms
  uint8_t buffer[XL_BUFFER_SIZE];
}XL_Interface;

typedef struct XL_S{
  uint8_t id;
  XL_Interface interface;
}XL;


typedef struct XL_Instruction_Packet_S{
  uint8_t id;
  XL_Instruction instruction;
  uint8_t nb_params;
  uint8_t *params;
}XL_Instruction_Packet;

typedef struct XL_Status_Packet_S{
  uint8_t id;
  uint8_t err;
  uint8_t nb_params;
  uint8_t params[XL_BUFFER_SIZE];
}XL_Status_Packet;

//Réception d'un paquet
typedef struct XL_Receiver_FSM_S{
  XL_Receiver_FSM_State state;
  uint16_t length;
  uint8_t *buffer;
  uint8_t *p_buffer;
}XL_Receiver_FSM;

void XL_Init_Receiver_FSM(XL_Receiver_FSM *fsm, uint8_t buffer[XL_BUFFER_SIZE]);
void XL_Update_Receiver_FSM(XL_Receiver_FSM *fsm, uint8_t byte);
uint8_t XL_Extract_Status_Packet(XL_Status_Packet *packet, uint8_t frame[XL_BUFFER_SIZE], uint16_t length);
uint8_t XL_Receive(XL_Interface *interface, XL_Status_Packet *packet, uint32_t timeout);

//Envoi d'un paquet
uint8_t XL_Build_Frame(XL_Instruction_Packet *packet, uint8_t buffer[XL_BUFFER_SIZE]);
/*
 * Construit une trame à partir d'un paquet instruction et l'écrit dans le buffer.
 * La fonction retourne la taille effective de la trame construite en cas de succès, 0 en cas d'erreur.
 */
uint8_t XL_Send(XL_Interface *interface, XL_Instruction_Packet *packet, uint32_t timeout);

//Contrôle de redondance cyclique
uint16_t XL_Update_CRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

#endif
