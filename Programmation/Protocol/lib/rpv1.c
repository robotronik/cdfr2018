#include "rpv1.h"

#define RP_EOF 0x00
#define WRITE_BUFF(p_buffer,byte) *(p_buffer++) = byte;
#define ENCODE_BYTE(byte,p_buffer,p_code) {\
    if(byte == RP_EOF){			   \
      byte = p_buffer - p_code;		   \
      *p_code = byte;			   \
      p_code = p_buffer++;		   \
    }					   \
    else{				   \
      *(p_buffer++) = byte;		   \
    }					   \
  }

void RP_Init_Interface(RP_Interface *interface, uint8_t (*send)(uint8_t *, uint16_t, uint32_t), uint8_t (*receive)(uint8_t *, uint16_t, uint32_t)){
  interface->send = send;
  interface->receive = receive;

  //FSM
  interface->fsm.update_state = RP_FSM_INIT;
  interface->fsm.out = interface->packet.data;
  interface->fsm.p_out = interface->fsm.out;
  interface->fsm.crc_accum = 0;
  interface->fsm.valid = 0;
}

uint8_t RP_Build_Frame(RP_Packet *packet, uint8_t buffer[RP_BUFFER_SIZE]){
  if((packet->len <= 0) || (packet->len + 5 > RP_BUFFER_SIZE) || (packet->len > RP_MAX_PACKET_SIZE)){
    return 0;
  }

  uint8_t *p_buffer = buffer;
  uint8_t *p_code = p_buffer++;
  uint16_t crc_accum = CRC_INIT;

  //Size (remaining bytes)
  const uint8_t size = 3 + packet->len;
  *(p_buffer++) = size;
  UPDATE_CRC(crc_accum, size);
  
  int i;
  for(i = 0; i < packet->len; i++){
    uint8_t byte = packet->data[i];
    UPDATE_CRC(crc_accum,byte);
    ENCODE_BYTE(byte,p_buffer, p_code);
  }
  
  //CRC-16
  uint8_t crc_low = crc_accum & 0xFF;
  uint8_t crc_high = crc_accum >> 8;
  ENCODE_BYTE(crc_low, p_buffer, p_code);
  ENCODE_BYTE(crc_high, p_buffer, p_code);
  
  //RP_EOF
  *p_code = p_buffer - p_code;
  *(p_buffer++) = RP_EOF;

  return p_buffer - buffer;
}

uint8_t RP_Send(RP_Interface *interface, RP_Packet *packet, uint32_t timeout){
  uint8_t len = RP_Build_Frame(packet, interface->buffer);
  interface->send(interface->buffer, len, timeout);

  return 0;
}

//========================================
//        FONCTIONS DE RECEPTION
//========================================
#define FSM_UPDATE(fsm,state_function) fsm->update_state = state_function

#define FSM_DECL_BYTE(fsm) uint8_t byte = *(fsm->in)
#define FSM_BYTE byte

#define FSM_RESET(fsm,valid_b) {					\
    FSM_UPDATE(fsm, RP_FSM_INIT);					\
    fsm->crc_accum = 0;							\
    fsm->valid = valid_b;						\
    fsm->p_out = fsm->out;						\
    return;								\
  }

#define FSM_CHECK_RESET(fsm,byte,valid_b) {		\
    if(byte == 0x00) {FSM_RESET(fsm,valid_b);}	\
  }

#define FSM_DECODE_BYTE(fsm, byte)			\
  (((--fsm->bs_count) == 0)?				\
   (fsm->bs_count = byte, (uint8_t) 0x00):		\
   (byte))


void RP_FSM_INIT(RP_Receiver_FSM *fsm){
  FSM_DECL_BYTE(fsm);
  FSM_CHECK_RESET(fsm, FSM_BYTE, false);
  
  fsm->bs_count = FSM_BYTE;
  FSM_UPDATE(fsm, RP_FSM_SIZE);
}

void RP_FSM_SIZE(RP_Receiver_FSM *fsm){
  FSM_DECL_BYTE(fsm);
  FSM_CHECK_RESET(fsm, FSM_BYTE, false);

  --(fsm->bs_count);
  UPDATE_CRC(fsm->crc_accum, FSM_BYTE);
  fsm->size = fsm->remaining = FSM_BYTE - 3;

  if(fsm->size > RP_MAX_PACKET_SIZE){
    FSM_RESET(fsm, false);
  }
  
  FSM_UPDATE(fsm, RP_FSM_DATA);
}

void RP_FSM_DATA(RP_Receiver_FSM *fsm){
  FSM_DECL_BYTE(fsm);
  FSM_CHECK_RESET(fsm, FSM_BYTE, false);

  FSM_BYTE = FSM_DECODE_BYTE(fsm, FSM_BYTE);
  *(fsm->p_out++) = FSM_BYTE;
  UPDATE_CRC(fsm->crc_accum, FSM_BYTE);

  if(!(--fsm->remaining)){
    FSM_UPDATE(fsm, RP_FSM_CRC_LOW);
  }
}

void RP_FSM_CRC_LOW(RP_Receiver_FSM *fsm){
  FSM_DECL_BYTE(fsm);
  FSM_CHECK_RESET(fsm, FSM_BYTE, false);

  if((fsm->crc_accum & 0xFF) != FSM_DECODE_BYTE(fsm, FSM_BYTE)){
    FSM_RESET(fsm, false);
  }else{
    FSM_UPDATE(fsm, RP_FSM_CRC_HIGH);
  }
}

void RP_FSM_CRC_HIGH(RP_Receiver_FSM *fsm){
  FSM_DECL_BYTE(fsm);
  FSM_CHECK_RESET(fsm, FSM_BYTE, false);

  if((fsm->crc_accum >> 8) != FSM_DECODE_BYTE(fsm, FSM_BYTE)){
    FSM_RESET(fsm, false);
  }else{
    FSM_UPDATE(fsm, RP_FSM_END);
  }
}

void RP_FSM_END(RP_Receiver_FSM *fsm){
  FSM_DECL_BYTE(fsm);
  FSM_RESET(fsm, (FSM_BYTE == 0x00));
}
