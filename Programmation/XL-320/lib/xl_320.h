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

//==================================================
//                INSTRUCTIONS
//==================================================
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

//==================================================
//           INTERFACE, RECEPTION, ENVOI
//==================================================
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

//Contrôle de redondance cyclique
uint16_t XL_Update_CRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

//Machine à états pour la réception
typedef struct XL_Receiver_FSM_S{
  void (*update_state)(struct XL_Receiver_FSM_S*);
  uint16_t remaining_bytes;//Octets restants lors de la réception
  uint8_t *buffer;
  uint8_t *p_buffer;//Curseur dans le buffer
  uint8_t done;
}XL_Receiver_FSM;

void XL_FSM_HEADER_0(XL_Receiver_FSM *fsm);
void XL_FSM_HEADER_1(XL_Receiver_FSM *fsm);
void XL_FSM_HEADER_2(XL_Receiver_FSM *fsm);
void XL_FSM_HEADER_3(XL_Receiver_FSM *fsm);
void XL_FSM_ID(XL_Receiver_FSM *fsm);
void XL_FSM_LENGTH_LOW(XL_Receiver_FSM *fsm);
void XL_FSM_LENGTH_HIGH(XL_Receiver_FSM *fsm);
void XL_FSM_INSTRUCTION(XL_Receiver_FSM *fsm);
void XL_FSM_RECEIVING(XL_Receiver_FSM *fsm);

typedef enum XL_Direction_E{
  XL_SEND, XL_RECEIVE
}XL_Direction;

typedef struct XL_Interface_S{
  uint8_t (*send)(uint8_t *, uint16_t, uint32_t);//data, size, timeout_ms
  uint8_t (*receive)(uint8_t *, uint16_t, uint32_t);//data, size, timeout ms
  void (*set_direction)(XL_Direction);
  XL_Receiver_FSM fsm;
  uint8_t buffer[XL_BUFFER_SIZE];
}XL_Interface;

uint8_t XL_Extract_Status_Packet(XL_Status_Packet *packet, uint8_t frame[XL_BUFFER_SIZE], uint16_t length);
/*
 * Extrait un paquet d'une trame de taille length contenant un paquet.
 * Renvoie 0 en cas de succès, 1 en cas d'échec.
 */

uint8_t XL_Receive(XL_Interface *interface, XL_Status_Packet *packet, uint16_t packet_size, uint32_t timeout);
/*
 * Reçoit un paquet depuis l'interface *interface et le stocke dans *packet dans le temps imparti (timeout).
 * Renvoie 0 en cas de succès, 1 en cas d'échec.
 */

//Envoi d'un paquet
uint8_t XL_Build_Frame(XL_Instruction_Packet *packet, uint8_t buffer[XL_BUFFER_SIZE]);
/*
 * Construit une trame à partir d'un paquet instruction et l'écrit dans le buffer.
 * La fonction retourne la taille effective de la trame construite en cas de succès, 0 en cas d'erreur.
 */
uint8_t XL_Send(XL_Interface *interface, XL_Instruction_Packet *packet, uint32_t timeout);


//==================================================
//                     XL-320
//==================================================
typedef struct XL_S{
  uint8_t id;
  uint8_t rom_lock;
  XL_Interface interface;
}XL;

typedef enum XL_Baud_Rate_E{
  XL_BAUD_RATE_9600 = 0, XL_BAUD_RATE_57600 = 1, XL_BAUD_RATE_115200 = 2, XL_BAUD_RATE_1MBPS = 3,
}XL_Baud_Rate;

typedef enum XL_Mode_E{
  XL_JOIN_MODE = 2, XL_WHEEL_MODE = 1
}XL_Mode;

typedef enum XL_Return_Level_E{
  XL_PING_RETURN = 0, XL_READ_RETURN = 1, XL_ALL_RETURN = 2
}XL_Return_Level;

typedef enum XL_Alarm_Shutdown_E{
  XL_ERROR_INPUT_VOLTAGE = 4, XL_ERROR_OVER_HEATING = 2, XL_ERROR_OVERLOAD = 1, XL_ERROR_OVER_9000 = 7
}XL_Alarm_Shutdown;

//======================================
//    DEFINITIONS TABLE DE CONTROLE       
//======================================

typedef enum XL_Field_E{
  //EEPROM - Configuration
  XL_ID = 3,
  XL_BAUD_RATE = 4,
  XL_RETURN_DELAY_TIME = 5,
  XL_CW_ANGLE_LIMIT = 6,
  XL_CCW_ANGLE_LIMIT = 8,
  XL_CONTROL_MODE = 11,
  XL_LIMIT_TEMPERATURE = 12,
  XL_LOWER_LIMIT_VOLTAGE = 13,
  XL_UPPER_LIMIT_VOLTAGE = 14,
  XL_MAX_TORQUE = 15,
  XL_RETURN_LEVEL = 17,
  XL_ALARM_SHUTDOWN = 18,
  //EEPROM - Info
  XL_MODEL_NUMBER = 0,
  XL_FIRMWARE_VERSION = 2,
  //RAM - Contrôle
  XL_TORQUE_ENABLE = 24,
  XL_LED = 25,
  XL_D_GAIN = 27,
  XL_I_GAIN = 28,
  XL_P_GAIN = 29,
  XL_GOAL_POSITON = 30,
  XL_MOVING_SPEED = 32,
  XL_TORQUE_LIMIT = 35,
  XL_PUNCH = 51,
  //RAM - Info
  XL_CURRENT_POSITION = 37,
  XL_CURRENT_SPEED = 39,
  XL_CURRENT_LOAD = 41,
  XL_CURRENT_VOLTAGE = 45,
  XL_CURRENT_TEMPERATURE = 46,
  XL_REGISTERED_INSTRUCTION = 47,
  XL_MOVING = 49,
  XL_HARDWARE_ERROR_STATUS = 50,
}XL_Field;

//======================================
//       CONFIGURATION EEPROM   
//======================================
/***************************************************
 * REMARQUE GENERALE :
 * Torque Enable doit être à 0
 * pour configurer les champs EEPROM.
 ***************************************************/
uint8_t XL_Write(XL *servo, XL_Field field, uint16_t data, uint8_t size, uint8_t now);
/*
 * Ecrit la valeur data de size octets dans l'EEPROM.
 */

uint8_t XL_Configure_ID(XL *servo, uint8_t id);
/*
 * Permet de modifier l'ID d'un servo.
 * id doit être compris entre 0x00 et 0xFC (252).
 */

uint8_t XL_Configure_Baud_Rate(XL *servo, XL_Baud_Rate baud_rate);
/*
 * Configure le débit de transmission du servo.
 * Les valeurs possibles sont :
 * 0 : 9600bps
 * 1 : 57600 bps
 * 2 : 115200 bps
 * 3 : 1 Mbps
 */

uint8_t XL_Configure_Return_Delay_Time(XL *servo, uint8_t delay);
/*
 * Configure le délai entre le moment ou le servo reçoit une
 * instruction et le moment où il répond.
 * Valeurs possibles : 0x00 -> 0xFE (254)
 * Unité : 2 microsecondes
 */

uint8_t XL_Configure_CW_Angle_Limit(XL *servo, uint16_t angle);
/*
 * Configure la limite d'angle dans le sens horaire (cf. doc)
 * Valeurs possibles : 0x000 -> 0x3FF (1023)
 */

uint8_t XL_Configure_CCW_Angle_Limit(XL *servo, uint16_t angle);
/*
 * Configure la limite d'angle dans le sens anti-horaire (cf. doc)
 * Valeurs possibles : 0x000 -> 0x3FF (1023)
 */

uint8_t XL_Configure_Control_Mode(XL *servo, XL_Mode mode);
/*
 * Configure le servomoteur en mode Joint ou Wheel.
 * 1 : Wheel mode, le servomoteur fixe une vitesse
 * 2 : Joint mode, le servomoteur fixe un angle
 * ATTENTION : CW_Angle_Limit et CCW_Angle_Limit doivent être
 * à 0 pour le mode Wheel, différents de 0 pour le mode Joint.
*/

uint8_t XL_Configure_Limit_Temperature(XL *servo, uint8_t temp);
/*
 * Configure la température max du servo avant détection d'une erreur
 * ERROR_OVER_HEATING.
 * Valeurs possibles : 0 -> 150
 * Cette fonction ne devrait pas exister.
 */

uint8_t XL_Configure_Lower_Limit_Voltage(XL *servo, uint8_t voltage);
/*
 * Configure la limite inférieure de tension avant détection d'une
 * erreur ERROR_INPUT_VOLTAGE.
 * Valeurs possibles : 50 -> 250 (0x32 -> 0x96)
 * Unité : 0.1 V
 */

uint8_t XL_Configure_Upper_Limit_Voltage(XL *servo, uint8_t voltage);
/*
 * Configure la limite supérieure de tension avant détection d'une
 * erreur ERROR_INPUT_VOLTAGE.
 * Valeurs possibles : 50 -> 250 (0x32 -> 0x96)
 * Unité : 0.1 V
 * Remarque : Les vrais mettront du triphasé
 */

uint8_t XL_Configure_Max_Torque(XL *servo, uint16_t max_torque);
/*
 * Configure le couple maximum du servomoteur.
 * Valeurs possibles : 0 -> 1023 (0% -> 100%)
 * Remarque : Cette fonction impose une limite mais
 * ne contrôle pas le couple, cf. Goal Torque (RAM).
 */

uint8_t XL_Configure_Return_Level(XL *servo, XL_Return_Level level);
/*
 * Configure le niveau de retour du servomoteur.
 * Valeurs possibles :
 * 0 : Aucun retour, sauf ping
 * 1 : Retour uniquement pour les commandes READ
 * 2 : Retour pour toutes les commandes
 * Remarque : Pour l'instant cette librairie ne fonctionne
 * correctement que dans le mode 1
*/

uint8_t XL_Configure_Alarm_Shutdown(XL *servo, XL_Alarm_Shutdown alarm);
/*
 * Configure les alarmes permettant de stopper le servomoteur en cas de problème.
 * Valeurs possibles : comibinaisons (OR) de
 * XL_ERROR_INPUT_VOLTAGE
 * XL_ERROR_OVER_HEATING
 * XL_ERROR_OVERLOAD
 * XL_ERROR_OVER_9000
 * ASTUCE DE HACKER : Incroyable ! Grâce à cette fonction, transformez
 * votre XL 320 en une superbe guirlande ! Vous n'en croirez pas vos
 * yeux !
 */

//======================================
//       COMMANDES SERVOMOTEUR       
//======================================

typedef enum XL_LED_Color_E{
  XL_LED_OFF = 0,
  XL_RED = 1,
  XL_GREEN = 2,
  XL_BLUE = 3,
  XL_YELLOW = XL_RED | XL_GREEN,
  XL_PINK = XL_RED | XL_BLUE,
  XL_CYAN = XL_GREEN | XL_BLUE,
  XL_WHITE = XL_RED | XL_GREEN | XL_BLUE
}XL_LED_Color;

uint8_t XL_Power_On(XL *servo, uint8_t now);
/*
 * Active le moteur.
 */

uint8_t XL_Power_Off(XL *servo, uint8_t now);
/*
 * Désactive le moteur.
 */

uint8_t XL_Set_LED(XL *servo, XL_LED_Color color, uint8_t now);
/*
 * Allume la LED du servo selon la couleur choisie.
 */

uint8_t XL_Set_D_Gain(XL *servo, uint8_t d_gain, uint8_t now);
/*
 * Modifie le gain en dérivée du régulateur PID.
 * Valeurs : 0 -> 254
 */

uint8_t XL_Set_I_Gain(XL *servo, uint8_t i_gain, uint8_t now);
/*
 * Modifie le gain en intégration du régulateur PID.
 * Valeurs : 0 -> 254
 */

uint8_t XL_Set_P_Gain(XL *servo, uint8_t p_gain, uint8_t now);
/*
 * Modifie le gain proportionnel du régulateur PID.
 * Valeurs : 0 -> 254
 */

uint8_t XL_Set_Goal_Position(XL *servo, uint16_t position, uint8_t now);
/*
 * Modifie la position cible du servo.
 * Valeurs possibles : 0 -> 1023 (0x3FF)
 * Unité : 0.29°
 * Remarque :
 * Cette valeur n'a un sens que si servo est en mode JOIN.
 * cf. doc
 */

typedef enum XL_Wheel_Direction_E{
  XL_CLOCKWISE, XL_COUNTERCLOCKWISE
}XL_Wheel_Direction;

uint8_t XL_Set_Goal_Speed(XL *servo, uint16_t speed, uint8_t now);
/*
 * Modifie la vitesse cible du servo.
 * Valeurs possibles :
 * JOIN Mode : 0 -> 1023, unité : 0.111 RPM, 0 : pas de contrôle de vitesse
 * WHEEL Mode :
 * 0 -> 1023 : sens anti-horaire
 * ...
 */

/*
  XL_TORQUE_LIMIT = 35,
  XL_PUNCH = 51,*/

#endif
