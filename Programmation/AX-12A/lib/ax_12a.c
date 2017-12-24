#include "ax_12a.h"
#include <stdio.h>

//========================================
//    CONSTANTES ET VARIABLES GLOBALES
//========================================
static const uint8_t header[2] = {0xFF, 0xFF};
static const uint8_t field_addr[] = {
  [AX_ID] = 3,
  [AX_BAUD_RATE] = 4,
  [AX_RETURN_DELAY_TIME] = 5,
  [AX_CW_ANGLE_LIMIT] = 6,
  [AX_CCW_ANGLE_LIMIT] = 8,
  [AX_CONTROL_MODE] = 11,
  [AX_LIMIT_TEMPERATURE] = 12,
  [AX_LOWER_LIMIT_VOLTAGE] = 13,
  [AX_UPPER_LIMIT_VOLTAGE] = 14,
  [AX_MAX_TORQUE] = 15,
  [AX_RETURN_LEVEL] = 17,
  [AX_ALARM_SHUTDOWN] = 18,
  //EEPROM - Info
  [AX_MODEL_NUMBER] = 0,
  [AX_FIRMWARE_VERSION] = 2,
  //RAM - Contrôle
  [AX_TORQUE_ENABLE] = 24,
  [AX_LED] = 25,
  [AX_D_GAIN] = 27,
  [AX_I_GAIN] = 28,
  [AX_P_GAIN] = 29,
  [AX_GOAL_POSITION] = 30,
  [AX_MOVING_SPEED] = 32,
  [AX_TORQUE_LIMIT] = 35,
  [AX_PUNCH] = 51,
  //RAM - Info
  [AX_CURRENT_POSITION] = 37,
  [AX_CURRENT_SPEED] = 39,
  [AX_CURRENT_LOAD] = 41,
  [AX_CURRENT_VOLTAGE] = 45,
  [AX_CURRENT_TEMPERATURE] = 46,
  [AX_REGISTERED_INSTRUCTION] = 47,
  [AX_MOVING] = 49,
  [AX_HARDWARE_ERROR_STATUS] = 50,
};
static const uint8_t field_length[] = {
  [AX_ID] = 1,
  [AX_BAUD_RATE] = 1,
  [AX_RETURN_DELAY_TIME] = 1,
  [AX_CW_ANGLE_LIMIT] = 2,
  [AX_CCW_ANGLE_LIMIT] = 2,
  [AX_CONTROL_MODE] = 1,
  [AX_LIMIT_TEMPERATURE] = 1,
  [AX_LOWER_LIMIT_VOLTAGE] = 1,
  [AX_UPPER_LIMIT_VOLTAGE] = 1,
  [AX_MAX_TORQUE] = 2,
  [AX_RETURN_LEVEL] = 1,
  [AX_ALARM_SHUTDOWN] = 1,
  //EEPROM - Info
  [AX_MODEL_NUMBER] = 2,
  [AX_FIRMWARE_VERSION] = 1,
  //RAM - Contrôle
  [AX_TORQUE_ENABLE] = 1,
  [AX_LED] = 1,
  [AX_D_GAIN] = 1,
  [AX_I_GAIN] = 1,
  [AX_P_GAIN] = 1,
  [AX_GOAL_POSITION] = 2,
  [AX_MOVING_SPEED] = 2,
  [AX_TORQUE_LIMIT] = 2,
  [AX_PUNCH] = 2,
  //RAM - Info
  [AX_CURRENT_POSITION] = 2,
  [AX_CURRENT_SPEED] = 2,
  [AX_CURRENT_LOAD] = 2,
  [AX_CURRENT_VOLTAGE] = 1,
  [AX_CURRENT_TEMPERATURE] = 1,
  [AX_REGISTERED_INSTRUCTION] = 1,
  [AX_MOVING] = 1,
  [AX_HARDWARE_ERROR_STATUS] = 1,
};
static uint16_t err;

//========================================
//        FONCTIONS DE RECEPTION
//========================================
#define FSM_BYTE *(fsm->p_buffer)
#define FSM_NEXT fsm->p_buffer++
#define FSM_RESET fsm->update_state = AX_FSM_HEADER_0; fsm->p_buffer = fsm->buffer
#define FSM_UPDATE(state_function) fsm->update_state = state_function

void AX_FSM_HEADER_0(AX_Receiver_FSM *fsm){
  if(FSM_BYTE == header[0]){
    FSM_NEXT;
    FSM_UPDATE(AX_FSM_HEADER_1);
  }
}

void AX_FSM_HEADER_1(AX_Receiver_FSM *fsm){
  if(FSM_BYTE == header[1]){
    FSM_NEXT;
    FSM_UPDATE(AX_FSM_ID);
  }
  else{
    FSM_RESET;
  }
}

void AX_FSM_ID(AX_Receiver_FSM *fsm){
  if(FSM_BYTE != 0xFF){
    FSM_NEXT;
    FSM_UPDATE(AX_FSM_LENGTH);
  }
  else{
    FSM_RESET;
  }
}

void AX_FSM_LENGTH(AX_Receiver_FSM *fsm){
  fsm->remaining_bytes = FSM_BYTE;
  const uint8_t remaining_bytes = fsm->remaining_bytes;
  if((remaining_bytes >= 2) && (4 + remaining_bytes <= AX_BUFFER_SIZE)){
    FSM_NEXT;
    FSM_UPDATE(AX_FSM_RECEIVING);
  }
  else{
    FSM_RESET;
  }
}

void AX_FSM_RECEIVING(AX_Receiver_FSM *fsm){
  FSM_NEXT;
  fsm->remaining_bytes--;
  if(fsm->remaining_bytes == 0){
    fsm->done = 1;
  }
}

uint8_t AX_Extract_Status_Packet(AX_Status_Packet *packet, uint8_t frame[AX_BUFFER_SIZE], uint16_t packet_size){
  //The packet size is supposed to be checked in AX_Receive function.
  //Checksum
  if(AX_Compute_Checksum(frame, packet_size-1) != frame[packet_size-1]){
    err = AX_ERR_LINK | AX_ERR_BAD_FRAME;
    return 1;
  }

  packet->id = frame[2];
  packet->err = frame[4];

  //Paramètres
  uint8_t *p_frame = frame+5;
  uint8_t *end_frame = frame+(packet_size-1);
  while(p_frame != end_frame){
    packet->params[packet->nb_params++] = *p_frame++;
  }
  
  return 0;
}

uint8_t AX_Receive(AX_Interface *interface, uint16_t packet_size, uint32_t timeout){
  //Avoid overflow
  if(packet_size > AX_BUFFER_SIZE){
    err = AX_ERR_INTERNAL | AX_ERR_BUFFER_OVERFLOW;
    return 1;
  }
  
  //Préparation de la réception
  interface->set_direction(AX_RECEIVE);

  //FSM initialization
  interface->fsm.update_state = AX_FSM_HEADER_0;
  interface->fsm.remaining_bytes = 0;
  interface->fsm.buffer = interface->buffer;
  interface->fsm.p_buffer = interface->buffer;
  interface->fsm.done = 0;

  //Reception
  if(interface->receive(interface->fsm.p_buffer, packet_size, timeout) != 0){
    err = AX_ERR_LINK | AX_ERR_TIMEOUT;
    return 1;
  }

  //FSM
  do{
    interface->fsm.update_state(&(interface->fsm));

    /*uint8_t *p;
    printf("Buffer : ");
    for(p = interface->buffer; p < interface->fsm.p_buffer; p++){
      printf("0x%2.2X ", *p);
    }
    printf("\n");*/
  }while(interface->fsm.p_buffer != interface->fsm.buffer && interface->fsm.done != 1);//If FSM is still working and has not reset, continue

  //Checking that the frame is valid
  if(interface->fsm.done == 0){
    err = AX_ERR_LINK | AX_ERR_BAD_FRAME;
    return 1;
  }

  //Checking the length of the packet
  uint16_t r_packet_size = interface->fsm.p_buffer - interface->fsm.buffer;
  if(r_packet_size != packet_size){
    err = AX_ERR_LINK | AX_ERR_BAD_FRAME;
    return 1;
  }

  //Extract the packet, then return
  return AX_Extract_Status_Packet(&interface->status, interface->buffer, packet_size);
}

//======================================
//           FONCTIONS D'ENVOI          
//======================================
uint8_t AX_Build_Frame(AX_Instruction_Packet *packet, uint8_t buffer[AX_BUFFER_SIZE]){
  //Verifying arguments
  if(packet == 0 || (packet->params == 0 && packet->nb_params > 0) || buffer == 0){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 0;
  }
  //and ID range
  if(packet->id == 0xFF){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 0;
  }
  //Avoid overflow
  if(6+packet->nb_params > AX_BUFFER_SIZE){
    err = AX_ERR_INTERNAL | AX_ERR_BUFFER_OVERFLOW;
    return 0;
  }

  uint8_t *p_buffer = buffer;
  //Header
  *(p_buffer++) = header[0];
  *(p_buffer++) = header[1];

  //Packet ID
  *(p_buffer++) = packet->id;

  //Packet length
  *(p_buffer++) = packet->nb_params+2;

  //Instruction
  *(p_buffer++) = packet->instruction;

  //Parameters
  int i;
  for(i=0; i < packet->nb_params; i++){
    *(p_buffer++) = packet->params[i];
  }

  //Checksum
  uint8_t checksum = AX_Compute_Checksum(buffer, p_buffer-buffer);
  *(p_buffer++) = checksum; 
  
  return p_buffer-buffer;    
}

uint8_t AX_Send(AX_Interface *interface, AX_Instruction_Packet *packet, uint32_t timeout){
  //Préparation de la trame
  uint8_t length = AX_Build_Frame(packet, interface->buffer);
  if(!length){
    return 1;
  }

  //Envoi
  interface->set_direction(AX_SEND);
  if(interface->send(interface->buffer, length, timeout) == 1){
    err = AX_ERR_LINK | AX_ERR_TIMEOUT;
    return 1;
  }
  return 0;
}

uint8_t AX_Compute_Checksum(uint8_t *data_blk_ptr, uint16_t data_blk_size){
  uint8_t checksum = 0;
  uint16_t i;
  for(i=0; i < data_blk_size; i++){
    checksum += data_blk_ptr[i];
  }
  return ~checksum;
}

//======================================
//         GESTION DES ERREURS      
//======================================
uint16_t AX_Get_Error(){
  return err;
}

uint8_t AX_Check_Status(AX *servo){
  if(AX_STATUS_ERROR(servo->interface->status.err) != 0){
    err = AX_ERR_STATUS | AX_STATUS_ERROR(servo->interface->status.err);
    return 1;
  }
  return 0;
}

uint8_t AX_Check_Alert(AX *servo){
  if(AX_STATUS_ALERT(servo->interface->status.err) == 1){
    uint16_t hw_error;
    if(AX_Get_Hardware_Error(servo, &hw_error) == 1){
      return 1;
    }
    err = AX_ERR_HARDWARE | hw_error;
    return 1;
  }
  return 0;
}


//======================================
//         JEU D'INSTRUCTIONS      
//======================================
uint8_t AX_Ping(AX *servo){
  if(servo == 0){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  
  AX_Instruction_Packet packet;
  packet.id = servo->id;
  packet.instruction = AX_PING;
  packet.nb_params = 0;
  packet.params = 0;

  //Envoi de l'instruction
  if(AX_Send(servo->interface, &packet, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //Réception de la réponse
  if(AX_Receive(servo->interface, 14, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //Vérification de la réponse
  return AX_Check_Status(servo);
}

uint8_t AX_Discover(AX_Interface *interface, AX *buffer_servos, uint8_t len_buffer, uint16_t *nb_servos){
  AX servo;
  servo.interface = interface;

  *nb_servos = 0;
  for(servo.id = 0x00; servo.id < 0xFD && (*nb_servos < len_buffer); servo.id++){
    if(AX_Ping(&servo) == 0){
      buffer_servos[(*nb_servos)++] = servo;
    }
  }
  
  return (*nb_servos > 0)?0:1;
}

uint8_t AX_Say_Hello(AX *servo){
  if(AX_Ping(servo) == 1){
    return 1;
  }

  const AX_LED_Color color = (servo->interface->status.err == 0x00)?AX_GREEN:AX_RED;
  uint8_t i;
  for(i = 0; i < servo->id; i++){
    AX_Set_LED(servo, color, AX_NOW);
    servo->interface->delay(500);
    AX_Set_LED(servo, AX_LED_OFF, AX_NOW);
    servo->interface->delay(500);
  }

  return 0;
}

uint8_t AX_Read(AX *servo, AX_Field field, uint16_t *data){
  if(field > AX_NB_FIELDS-1){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  
  //Préparation de l'instruction READ
  AX_Instruction_Packet packet;
  packet.id = servo->id;
  packet.instruction = AX_READ;
  packet.nb_params = 4;
  uint8_t params[4] = {field_addr[field], 0x00, field_length[field], 0x00};
  packet.params = params;

  //Envoi de l'instruction
  if(AX_Send(servo->interface, &packet, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }
  
  //Réception de la réponse
  if(AX_Receive(servo->interface, 11+field_length[field], AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //Vérification de la réponse
  if(AX_Check_Status(servo) == 1){
    return 1;
  }
  
  //Récupération de la donnée
  *data = servo->interface->status.params[0];
  if(field_length[field] == 2){
    *data |= servo->interface->status.params[1] << 8;
  }
  return 0;
}

uint8_t AX_Action(AX *servo){
  //Préparation de l'instruction
  AX_Instruction_Packet packet;
  packet.id = servo->id;
  packet.instruction = AX_ACTION;
  packet.nb_params = 0;
  packet.params = 0;

  //Envoi de l'instruction
  if(AX_Send(servo->interface, &packet, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //Réception de la réponse
  if(AX_Receive(servo->interface, 11, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //En cas d'erreur matérielle
  if(AX_Check_Alert(servo) == 1){
    return 1;
  }

  //Vérification du status
  if(AX_Check_Status(servo) == 1){
    return 1;
  }

  //Action OK
  return 0;
}

uint8_t AX_Factory_Reset(AX *servo){
  AX_Instruction_Packet packet;
  packet.id = servo->id;
  packet.instruction = AX_FACTORY_RESET;
  packet.nb_params = 1;
  uint8_t param = 0x02;
  packet.params = &param;

  //Envoi de l'instruction
  if(AX_Send(servo->interface, &packet, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //Attente de la réinitialisation
  servo->interface->delay(5000);
  
  return 0;
}

uint8_t AX_Reboot(AX *servo){
  AX_Instruction_Packet packet;
  packet.id = servo->id;
  packet.instruction = AX_REBOOT;
  packet.nb_params = 0;
  packet.params = 0;

  //Envoi de l'instruction
  if(AX_Send(servo->interface, &packet, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //Attente du redémarrage
  servo->interface->delay(5000);
  
  return 0;
}

uint8_t AX_Write(AX *servo, AX_Field field, uint16_t data, uint8_t size, uint8_t now){
  if(servo == 0 || field > (AX_NB_FIELDS-1)){
    return 1;
  }

  //Préparation de l'instruction
  static AX_Instruction_Packet packet;
  static uint8_t params[4];
  params[0] = field_addr[field];
  params[1] = 0x00;
  params[2] = data & 0xFF;
  params[3] = data >> 8;
  packet.id = servo->id;
  packet.instruction = now?AX_WRITE:AX_REG_WRITE;
  packet.nb_params = 2 + size;
  packet.params = params;

  //Envoi de l'instruction
  if(AX_Send(servo->interface, &packet, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //Réception de la réponse
  if(AX_Receive(servo->interface, 11, AX_DEFAULT_TIMEOUT) == 1){
    return 1;
  }

  //En cas d'erreur matérielle
  if(AX_Check_Alert(servo) == 1){
    return 1;
  }

  //Vérification du status
  if(AX_Check_Status(servo) == 1){
    return 1;
  }

  //Write OK
  return 0;
}

//======================================
//         CONFIGURATION EEPROM       
//======================================
uint8_t AX_Configure_ID(AX *servo, uint8_t id){
  if(id > 252){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_ID, id, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Baud_Rate(AX *servo, AX_Baud_Rate baud_rate){
  if(baud_rate != AX_BAUD_RATE_9600 && baud_rate != AX_BAUD_RATE_57600 && baud_rate != AX_BAUD_RATE_115200 && baud_rate != AX_BAUD_RATE_1MBPS){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_BAUD_RATE, baud_rate, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Return_Delay_Time(AX *servo, uint8_t delay){
  if(delay > 0xFE){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_RETURN_DELAY_TIME, delay, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_CW_Angle_Limit(AX *servo, uint16_t angle){
  if(angle > 0x3FF){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_CW_ANGLE_LIMIT, angle, 2, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_CCW_Angle_Limit(AX *servo, uint16_t angle){
  if(angle > 0x3FF){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_CCW_ANGLE_LIMIT, angle, 2, AX_NOW);
  servo->interface->delay(10);
  return r;
}
  
uint8_t AX_Configure_Control_Mode(AX *servo, AX_Mode mode){
  if(mode != AX_JOIN_MODE && mode != AX_WHEEL_MODE){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_CONTROL_MODE, mode, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Limit_Temperature(AX *servo, uint8_t temp){
  if(temp > 150){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_LIMIT_TEMPERATURE, temp, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Lower_Limit_Voltage(AX *servo, uint8_t voltage){
  if(voltage < 50 || voltage > 250){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_LOWER_LIMIT_VOLTAGE, voltage, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Upper_Limit_Voltage(AX *servo, uint8_t voltage){
  if(voltage < 50 || voltage > 250){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_UPPER_LIMIT_VOLTAGE, voltage, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Max_Torque(AX *servo, uint16_t max_torque){
  if(max_torque > 1023){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_MAX_TORQUE, max_torque, 2, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Return_Level(AX *servo, AX_Return_Level level){
  if(level != AX_PING_RETURN && level != AX_READ_RETURN && level != AX_ALL_RETURN){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_RETURN_LEVEL, level, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}

uint8_t AX_Configure_Alarm_Shutdown(AX *servo, AX_Alarm_Shutdown alarm){
  if(alarm > AX_ERROR_OVER_9000){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  uint8_t r = AX_Write(servo, AX_ALARM_SHUTDOWN, alarm, 1, AX_NOW);
  servo->interface->delay(10);
  return r;
}


//======================================
//       COMMANDES SERVOMOTEUR       
//======================================

uint8_t AX_Power_On(AX *servo, uint8_t now){
  return AX_Write(servo, AX_TORQUE_ENABLE, 1, 1, now);
}

uint8_t AX_Power_Off(AX *servo, uint8_t now){
  return AX_Write(servo, AX_TORQUE_ENABLE, 0, 1, now);
}

uint8_t AX_Set_LED(AX *servo, AX_LED_Color color, uint8_t now){
  if((uint8_t) color > 7){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_LED, color, 1, now);
}

uint8_t AX_Set_D_Gain(AX *servo, uint8_t d_gain, uint8_t now){
  if(d_gain > 254){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_D_GAIN, d_gain, 1, now);
}

uint8_t AX_Set_I_Gain(AX *servo, uint8_t i_gain, uint8_t now){
  if(i_gain > 254){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_I_GAIN, i_gain, 1, now);
}

uint8_t AX_Set_P_Gain(AX *servo, uint8_t p_gain, uint8_t now){
  if(p_gain > 254){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_P_GAIN, p_gain, 1, now);
}

uint8_t AX_Set_Goal_Position(AX *servo, uint16_t position, uint8_t now){
  if(position > 1023){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_GOAL_POSITION, position, 2, now);
}

uint8_t AX_Set_Goal_Speed_Join(AX *servo, uint16_t speed, uint8_t now){
  if(speed > 1023){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_MOVING_SPEED, speed, 2, now); 
}

uint8_t AX_Set_Goal_Speed_Wheel(AX *servo, uint16_t speed, AX_Wheel_Direction dir, uint8_t now){
  if(speed > 1023){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  switch(dir){
  case AX_CLOCKWISE:
    speed += 1024;
    break;
  case AX_COUNTERCLOCKWISE:
  break;
  default:
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
    break;
  }
  return AX_Write(servo, AX_MOVING_SPEED, speed, 2, now);
}

uint8_t AX_Set_Torque_Limit(AX *servo, uint16_t torque_limit, uint8_t now){
  if(torque_limit > 1023){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_TORQUE_LIMIT, torque_limit, 2, now);
}

uint8_t AX_Set_Punch(AX *servo, uint16_t punch, uint8_t now){
  if(punch < 0x20 || punch > 0x3FF){
    err = AX_ERR_INTERNAL | AX_ERR_ILLEGAL_ARGUMENTS;
    return 1;
  }
  return AX_Write(servo, AX_PUNCH, punch, 2, now);
}

//======================================
//       LECTURE D'INFORMATIONS
//======================================
uint8_t AX_Get_Current_Position(AX *servo, uint16_t *position){
  return AX_Read(servo, AX_CURRENT_POSITION, position);
}

uint8_t AX_Get_Current_Speed(AX *servo, AX_Wheel_Direction *direction, uint16_t *speed){
  if(AX_Read(servo, AX_CURRENT_SPEED, speed) == 1){
    return 1;
  }
  *direction = (*speed > 1023)?AX_CLOCKWISE:AX_COUNTERCLOCKWISE;
  *speed &= 0xFF;
  return 0;
}

uint8_t AX_Get_Current_Load(AX *servo, AX_Wheel_Direction *direction, uint16_t *load){
  if(AX_Read(servo, AX_CURRENT_LOAD, load) == 1){
    return 1;
  }
  *direction = (*load > 1023)?AX_CLOCKWISE:AX_COUNTERCLOCKWISE;
  *load &= 0xFF;
  return 0;
}

uint8_t AX_Get_Current_Voltage(AX *servo, uint16_t *voltage){
  return AX_Read(servo, AX_CURRENT_VOLTAGE, voltage);
}

uint8_t AX_Get_Current_Temperature(AX *servo, uint16_t *temperature){
  return AX_Read(servo, AX_CURRENT_TEMPERATURE, temperature);
}

uint8_t AX_Is_Working(AX *servo, uint16_t *working){
  return AX_Read(servo, AX_TORQUE_ENABLE, working);
}

uint8_t AX_Is_Moving(AX *servo, uint16_t *moving){
  return AX_Read(servo, AX_MOVING, moving);
}

uint8_t AX_Get_Hardware_Error(AX *servo, uint16_t *hw_error){
  return AX_Read(servo, AX_HARDWARE_ERROR_STATUS, hw_error);
}
