#ifndef __RPV1_H
#define __RPV1_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "crc_16.h"

#define RP_MAX_PACKET_SIZE 251
#define RP_BUFFER_SIZE RP_MAX_PACKET_SIZE+5

//Finite state machine for reception
typedef struct RP_Receiver_FSM_S{
  void (*update_state)(struct RP_Receiver_FSM_S*);
  uint8_t bs_count;//COBS counter
  uint8_t size;//packet size
  uint8_t remaining;
  uint8_t *in;
  uint8_t *out;
  uint8_t *p_out;
  uint16_t crc_accum;
  bool valid;
}RP_Receiver_FSM;

//Data packet
typedef struct RP_Packet_S{
  uint8_t len;
  uint8_t data[RP_MAX_PACKET_SIZE];
}RP_Packet;

typedef struct RP_Interface_S{
  uint8_t (*send)(uint8_t *, uint16_t, uint32_t);//data, size, timeout_ms
  uint8_t (*receive)(uint8_t *, uint16_t, uint32_t);//data, size, timeout ms
  uint8_t buffer[RP_BUFFER_SIZE];
  RP_Receiver_FSM fsm;
  RP_Packet packet;
}RP_Interface;

void RP_Init_Interface(RP_Interface *interface, uint8_t (*send)(uint8_t *, uint16_t, uint32_t), uint8_t (*receive)(uint8_t *, uint16_t, uint32_t));

void RP_FSM_INIT(RP_Receiver_FSM *fsm);
void RP_FSM_SIZE(RP_Receiver_FSM *fsm);
void RP_FSM_DATA(RP_Receiver_FSM *fsm);
void RP_FSM_CRC_LOW(RP_Receiver_FSM *fsm);
void RP_FSM_CRC_HIGH(RP_Receiver_FSM *fsm);
void RP_FSM_END(RP_Receiver_FSM *fsm);

uint16_t RP_Update_CRC(uint16_t accum, uint8_t *data, uint16_t size);

uint8_t RP_Build_Frame(RP_Packet *packet, uint8_t buffer[RP_BUFFER_SIZE]);
/**
 * Builds a frame from the packet and store it in the buffer. Returns
 * the effective size of the frame if it succeeds, 0 otherwise. The
 * frame is built according to the Consistent Overhead Byte Stuffing
 * algorithm.
 * Here is the frame format, by byte :
 * [COBS init] [size] [data] ...  [CRC-16_LOW] [CRC-16_HIGH] [EOF]
 * Size is (3 + packet->len).
 * The CRC is calculated on [size] and [data] ... bytes,
 * before byte stuffing.
 * Conditions :
 * (packet != NULL)
 * (packet->len > 0)
 * (packet->data != NULL)
 * (3 + packet->len <= 254)
 */

uint8_t RP_Send(RP_Interface *interface, RP_Packet *packet, uint32_t timeout);

//==================================================
//              GESTION DES ERREURS
//==================================================
//Types d'erreurs
typedef enum RP_Error_Type_E{
  RP_NO_ERROR = 0x0000, //Pas d'erreur
  RP_ERR_INTERNAL = 0x0100, //Erreur interne à la librairie
  RP_ERR_LINK = 0x0200, //Erreur de communication (UART)
}RP_Error_Type;

//Erreurs internes à la librairie
typedef enum RP_Internal_Error_E{
  RP_ERR_ILLEGAL_ARGUMENTS, //Quand l'utilisateur en a trop pris
  RP_ERR_BUFFER_OVERFLOW, //Quand le buffer ne s'est pas fait respecter
}RP_Internal_Error;

//Erreurs de communication (UART)
typedef enum RP_Link_Error_E{
  RP_ERR_TIMEOUT, //Quand on n'a pas eu le temps de se dépêcher
  RP_ERR_UNEXPECTED_EOF, //Le flag de fin de trame a été trouvé mais
			 //la trame était incomplète.
  RP_ERR_SIZE, //La taille de la trame est incorrecte
  RP_ERR_CRC, //Le CRC est incorrect
}RP_Link_Error;

//Macros sur les erreurs
#define RP_ERROR_TYPE(err) (RP_Error_Type) (err & 0xFF00)
#define RP_ERROR_CODE(err) (err & 0xFF)
#define RP_INTERNAL_ERROR(err) (RP_Internal_Error) RP_ERROR_CODE(err)
#define RP_LINK_ERROR(err) (RP_Link_Error) RP_ERROR_CODE(err)

//Fonction de récupération de l'erreur
uint16_t RP_Get_Error();

#endif
