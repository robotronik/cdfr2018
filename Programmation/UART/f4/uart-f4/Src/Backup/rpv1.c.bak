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

static uint16_t err;

void RP_Init_Interface(RP_Interface *interface,
		       uint8_t (*send)(uint8_t *, uint16_t, uint32_t)){
  interface->send = send;

  //FSM
  interface->fsm.update_state = RP_FSM_INIT;

  interface->fsm.in = interface->buffer_in;
  interface->fsm.p_in = interface->fsm.in;

  interface->fsm.out = interface->r_packet.data;
  interface->fsm.p_out = interface->fsm.out;

  interface->fsm.packet = &interface->r_packet;
  
  interface->fsm.crc_accum = 0;
}

uint8_t RP_Build_Frame(RP_Packet *packet, uint8_t buffer[RP_BUFFER_SIZE]){
  if((packet->len <= 0) || (packet->len + 5 > RP_BUFFER_SIZE) || (packet->len > RP_MAX_PACKET_SIZE)){
    err =  RP_ERR_INTERNAL | RP_ERR_ILLEGAL_ARGUMENTS;
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
  uint8_t len = RP_Build_Frame(packet, interface->buffer_out);

  if(len == 0){
    return 1;
  }
  
  return (interface->send(interface->buffer_out, len, timeout)==0);
}

//========================================
//        FONCTIONS DE RECEPTION
//========================================
#define FSM_UPDATE(fsm,state_function) fsm->update_state = state_function

#define FSM_GET_BYTE(fsm) uint8_t byte = *(fsm->p_in++);	\
  printf("0x%2.2X ", byte);					\
  if(fsm->p_in == fsm->in+RP_BUFFER_SIZE){			\
    fsm->p_in = fsm->in;					\
  }

#define FSM_BYTE byte

#define FSM_RESET(fsm) {						\
    FSM_UPDATE(fsm, RP_FSM_INIT);					\
    fsm->crc_accum = 0;							\
    fsm->p_out = fsm->out;						\
    return;								\
  }

#define FSM_ERR(fsm, err_code) {		\
  err = err_code;				\
  RP_Error_Handler(err);			\
  FSM_RESET(fsm);				\
  }

#define FSM_CHECK_ERR(fsm, err_code) {		\
    if(FSM_BYTE == RP_EOF){			\
      FSM_ERR(fsm, err_code);			\
    }						\
  }

#define FSM_CHECK_EOF(fsm) {			\
    if(FSM_BYTE == RP_EOF) {FSM_RESET(fsm);}	\
  }
/**
 * Reinit the FSM when EOF is reached.
 * Set the 'valid' flag of FSM
 * and the error code.
 */

#define FSM_DECODE_BYTE(fsm, byte)			\
  (((--fsm->bs_count) == 0)?				\
   (fsm->bs_count = byte, (uint8_t) RP_EOF):		\
   (byte))
/**
 * Update the COBS counter and decode the current byte. This macro can
 * be used once and only once in an fsm state. If the macro is not
 * used and the FSM is not reset, bs_count MUST be updated manually.
 */


void RP_FSM_INIT(RP_Receiver_FSM *fsm){
  FSM_GET_BYTE(fsm);
  /* 
   * If current byte is EOF, the fsm is reset but no error is set :
   * it's just a blank frame that can be used for synchronisation
   * purpose.
   */
  FSM_CHECK_EOF(fsm);

  /*
   * The first byte initializes the COBS algorithm.
   */
  fsm->bs_count = FSM_BYTE;

  FSM_UPDATE(fsm, RP_FSM_SIZE);
}

void RP_FSM_SIZE(RP_Receiver_FSM *fsm){
  FSM_GET_BYTE(fsm);
  /*
   * From this byte, reaching an EOF flag before the END state is
   * unexpected and will trigger an error.
   */
  FSM_CHECK_ERR(fsm, RP_ERR_LINK | RP_ERR_UNEXPECTED_EOF);

  /*
   * FSM_DECODE_BYTE is not used here because the protocol requires
   * that the size is > 0. So bs_count is decremented manually.
   */
  --(fsm->bs_count);

  /*
   * For the states RP_FSM_SIZE and RP_FSM_DATA, the CRC must be
   * updated for each byte.
   */
  UPDATE_CRC(fsm->crc_accum, FSM_BYTE);
  /*
   * Get the packet length. It will be used to find the CRC position.
   */
  fsm->size = fsm->remaining = FSM_BYTE - 3;
  fsm->packet->len = fsm->size;
  
  /*
   * Checks wether or not the read size is consistent.
   */
  if(fsm->size > RP_MAX_PACKET_SIZE){
    FSM_ERR(fsm, RP_ERR_LINK | RP_ERR_SIZE);
  }
  
  FSM_UPDATE(fsm, RP_FSM_DATA);
}

void RP_FSM_DATA(RP_Receiver_FSM *fsm){
  FSM_GET_BYTE(fsm);
  FSM_CHECK_ERR(fsm, RP_ERR_LINK | RP_ERR_UNEXPECTED_EOF);

  /*
   * Note that the CRC is computed AFTER the byte is decoded. Indeed,
   * the CRC concerns the useful data.
   */
  FSM_BYTE = FSM_DECODE_BYTE(fsm, FSM_BYTE);
  *(fsm->p_out++) = FSM_BYTE;
  UPDATE_CRC(fsm->crc_accum, FSM_BYTE);

  if(!(--fsm->remaining)){
    /*
     * According to the size read, the next bytes should contains CRC
     * information.
     */
    FSM_UPDATE(fsm, RP_FSM_CRC_LOW);
  }
}

void RP_FSM_CRC_LOW(RP_Receiver_FSM *fsm){
  FSM_GET_BYTE(fsm);
  FSM_CHECK_ERR(fsm, RP_ERR_LINK | RP_ERR_UNEXPECTED_EOF);

  /*
   * Checks the low byte of CRC, and triggers an error if it doesn't match.
   */
  if((fsm->crc_accum & 0xFF) != FSM_DECODE_BYTE(fsm, FSM_BYTE)){
    FSM_ERR(fsm, RP_ERR_LINK | RP_ERR_CRC);
  }else{
    FSM_UPDATE(fsm, RP_FSM_CRC_HIGH);
  }
}

void RP_FSM_CRC_HIGH(RP_Receiver_FSM *fsm){
  FSM_GET_BYTE(fsm);
  FSM_CHECK_ERR(fsm, RP_ERR_LINK | RP_ERR_UNEXPECTED_EOF);

  /*
   * Checks the high byte of CRC, and triggers an error if it doesn't match.
   */
  if((fsm->crc_accum >> 8) != FSM_DECODE_BYTE(fsm, FSM_BYTE)){
    FSM_ERR(fsm, RP_ERR_LINK | RP_ERR_CRC);
  }else{
    FSM_UPDATE(fsm, RP_FSM_END);
  }
}

void RP_FSM_END(RP_Receiver_FSM *fsm){
  FSM_GET_BYTE(fsm);

  /*
   * The last byte must be EOF. Otherwise, it is an error.
   */
  if(FSM_BYTE == 0x00){
    RP_Packet_Received(fsm->packet);
    FSM_RESET(fsm);
  }else{
    FSM_ERR(fsm, RP_ERR_LINK | RP_ERR_SIZE);
  }
}

uint16_t RP_Get_Error(){
  return err;
}
