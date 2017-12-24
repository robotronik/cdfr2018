/*
 * Library for AX-12A @Robotronik
 * ax_12a.h
 * Written for a little-endian µC
 * You may freely rage with this software
 */
#ifndef AX_12A_HEADER
#define AX_12A_HEADER

#include <stdint.h>

#define AX_BUFFER_SIZE 32
#define AX_DEFAULT_TIMEOUT 1
#define AX_BROADCAST 0xFE

//==================================================
//                INSTRUCTIONS
//==================================================
typedef enum AX_Instruction_E{
  AX_PING = 0x01,
  AX_READ = 0x02,
  AX_WRITE = 0x03,
  AX_REG_WRITE = 0x04,
  AX_ACTION = 0x05,
  AX_FACTORY_RESET = 0x06,
  AX_SYNC_WRITE = 0x83,
}AX_Instruction;

//==================================================
//           INTERFACE, RECEPTION, SENDING
//==================================================
typedef struct AX_Instruction_Packet_S{
  uint8_t id;
  AX_Instruction instruction;
  uint8_t nb_params;
  uint8_t *params;
}AX_Instruction_Packet;

typedef struct AX_Status_Packet_S{
  uint8_t id;
  uint8_t err;
  uint8_t nb_params;
  uint8_t params[AX_BUFFER_SIZE];
}AX_Status_Packet;

/*
 * Compute checksum for the data between data_blk_ptr and
 * data_blk_ptr+(data_blk_size-1).
 */
uint8_t AX_Compute_Checksum(uint8_t *data_blk_ptr, uint16_t data_blk_size);

//Final state machine used for reception
typedef struct AX_Receiver_FSM_S{
  void (*update_state)(struct AX_Receiver_FSM_S*);
  uint8_t remaining_bytes;//Remaining bytes while receiving.
  uint8_t *buffer;
  uint8_t *p_buffer;//Current position in the buffer
  uint8_t done;
}AX_Receiver_FSM;

void AX_FSM_HEADER_0(AX_Receiver_FSM *fsm);
void AX_FSM_HEADER_1(AX_Receiver_FSM *fsm);
void AX_FSM_ID(AX_Receiver_FSM *fsm);
void AX_FSM_LENGTH(AX_Receiver_FSM *fsm);
void AX_FSM_RECEIVING(AX_Receiver_FSM *fsm);

typedef enum AX_Direction_E{
  AX_SEND, AX_RECEIVE
}AX_Direction;

typedef struct AX_Interface_S{
  uint8_t (*send)(uint8_t *, uint16_t, uint32_t);//data, size, timeout_ms
  uint8_t (*receive)(uint8_t *, uint16_t, uint32_t);//data, size, timeout ms
  void (*set_direction)(AX_Direction);
  void (*delay)(uint32_t);//ms
  AX_Receiver_FSM fsm;
  uint8_t buffer[AX_BUFFER_SIZE];
  AX_Status_Packet status;
}AX_Interface;
/*
 * Note :
 * The interface's buffer is used both for sending and receiving packets.
 */

uint8_t AX_Extract_Status_Packet(AX_Status_Packet *packet, uint8_t frame[AX_BUFFER_SIZE], uint16_t packet_size);
/*
 * Extract a status packet from a valid frame wich total length is 'packet_size'.
 * Return 0 on success, 1 if it fails.
 */

uint8_t AX_Receive(AX_Interface *interface, uint16_t packet_size, uint32_t timeout);
/*
 * Receive a packet from *interface in the given time (timeout).
 * The received packet is stored in interface->status.
 * packet_size is the effective length of the packet, from header to checksum included.
 * This function should return 0 on success, 1 otherwise.
 */

//Sending a packet
uint8_t AX_Build_Frame(AX_Instruction_Packet *packet, uint8_t buffer[AX_BUFFER_SIZE]);
/*
 * Build a frame from an instruction packet and write it on the
 * buffer.  This function return the effective elngth of the built
 * frame on success, 0 if an error occured.
 */

uint8_t AX_Send(AX_Interface *interface, AX_Instruction_Packet *packet, uint32_t timeout);


//==================================================
//                     AX-12A
//==================================================
typedef struct AX_S{
  uint8_t id;
  AX_Interface *interface;
}AX;

typedef enum AX_Baud_Rate_E{
  AX_BAUD_RATE_9600 = 0, AX_BAUD_RATE_57600 = 1, AX_BAUD_RATE_115200 = 2, AX_BAUD_RATE_1MBPS = 3,
}AX_Baud_Rate;

typedef enum AX_Mode_E{
  AX_JOIN_MODE = 2, AX_WHEEL_MODE = 1
}AX_Mode;

typedef enum AX_Return_Level_E{
  AX_PING_RETURN = 0, AX_READ_RETURN = 1, AX_ALL_RETURN = 2
}AX_Return_Level;

typedef enum AX_Alarm_Shutdown_E{
  AX_ERROR_INPUT_VOLTAGE = 4, AX_ERROR_OVER_HEATING = 2, AX_ERROR_OVERLOAD = 1, AX_ERROR_OVER_9000 = 7
}AX_Alarm_Shutdown;

//======================================
//    DEFINITIONS TABLE DE CONTROLE       
//======================================

#define AX_NOW 1
#define AX_LATER 0

#define AX_NB_FIELDS 31
typedef enum AX_Field_E{
  //EEPROM - Configuration  
  AX_ID,
  AX_BAUD_RATE,
  AX_RETURN_DELAY_TIME,
  AX_CW_ANGLE_LIMIT,
  AX_CCW_ANGLE_LIMIT,
  AX_LIMIT_TEMPERATURE,
  AX_LOWER_LIMIT_VOLTAGE,
  AX_UPPER_LIMIT_VOLTAGE,
  AX_MAX_TORQUE,
  AX_RETURN_LEVEL,
  AX_ALARM_LED,
  AX_ALARM_SHUTDOWN,
  //EEPROM - Info
  AX_MODEL_NUMBER,
  AX_FIRMWARE_VERSION,
  //RAM - Contrôle
  AX_TORQUE_ENABLE,
  AX_LED,
  AX_CW_COMPLIANCE_MARGIN,
  AX_CCW_COMPLIANCE_MARGIN,
  AX_CW_COMPLIANCE_SLOPE,
  AX_CCW_COMPLIANCE_SLOPE,
  AX_GOAL_POSITION,
  AX_MOVING_SPEED,
  AX_TORQUE_LIMIT,
  AX_EEPROM_LOCK,
  AX_PUNCH,
  //RAM - Info
  AX_CURRENT_POSITION,
  AX_CURRENT_SPEED,
  AX_CURRENT_LOAD,
  AX_CURRENT_VOLTAGE,
  AX_CURRENT_TEMPERATURE,
  AX_REGISTERED_INSTRUCTION,
  AX_MOVING,
}AX_Field;

//======================================
//        JEU D'INSTRUCTIONS
//======================================
uint8_t AX_Write(AX *servo, AX_Field field, uint16_t data, uint8_t size, uint8_t now);
/*
 * Ecrit la valeur data de size octets dans la table de contrôle.
 * Remarque : Cette fonction n'effectue pas de vérifications sur data
 * et field, elle ne doit pas être appelée directement par un
 * utilisateur.
 */

uint8_t AX_Ping(AX *servo);
/*
 * Envoie une requête ping au servomoteur et récupère la réponse dans
 * servo->interface->status.  Renvoie 0 en cas de succès, 1 en cas
 * d'échec.
 */

uint8_t AX_Discover(AX_Interface *interface, AX *buffer_servos, uint8_t len_buffer, uint16_t *nb_servos);
/*
 * Envoie une requête ping en broadcast et récupère une liste d'au
 * plus len_buffer servomoteurs ayant répondu.  Met à jour nb_servo
 * pour indiquer le nombre de servomoteurs découverts.
 */

uint8_t AX_Say_Hello(AX *servo);
/*
 * Vérifie l'existence du servomoteur et fait clignoter sa LED autant
 * de fois que son ID, avec une fréquence de 1Hz.
 * La LED clignote en rouge s'il y a une erreur, en vert sinon.
 * Renvoie 0 si le servomoteur existe, -1 sinon.
 */

uint8_t AX_Read(AX *servo, AX_Field field, uint16_t *data);
/*
 * Lis une donnée dans la table de contrôle du servomoteur. En cas de
 * succès, le status return est stocké dans l'interface et la donnée
 * est récupérée dans data. 
 * Renvoie 0 en cas de succès, -1 en cas
 * d'échec.
 * Le AX_Status_Packet d'une interface n'est
 * consultable que jusqu'à la prochaine action.
 */

uint8_t AX_Action(AX *servo);
/*
 * Exécute l'action contenue dans le paquet d'instruction
 * préalablement écris sur le servo grâce à une instruction REG_WRITE
 * (AX_Write(...) avec now = 0).  Renvoie 0 en cas de succès, -1 en
 * cas d'échec.  Remarque : Cette fonction ne vérifie pas si l'action
 * a fonctionné, mais simplement si le paquet a pu être envoyé.
 */

uint8_t AX_Factory_Reset(AX *servo);
/*
 * Remet toutes les valeurs de l'EEPROM à leur valeur par défaut, sauf
 * l'ID et la vitesse de transmission.  Attention : il faut laisser au
 * servo environ 5 secondes pour se réinitialiser avant d'envoyer de
 * nouvelles instructions.
 */

//======================================
//       CONFIGURATION EEPROM   
//======================================
/***************************************************
 * REMARQUE GENERALE :
 * Torque Enable doit être à 0
 * pour configurer les champs EEPROM.
 ***************************************************/

uint8_t AX_Configure_ID(AX *servo, uint8_t id);
/*
 * Permet de modifier l'ID d'un servo.
 * id doit être compris entre 0x00 et 0xFC (252).
 */

uint8_t AX_Configure_Baud_Rate(AX *servo, AX_Baud_Rate baud_rate);
/*
 * Configure le débit de transmission du servo.
 * Les valeurs possibles sont :
 * 0 : 9600bps
 * 1 : 57600 bps
 * 2 : 115200 bps
 * 3 : 1 Mbps
 */

uint8_t AX_Configure_Return_Delay_Time(AX *servo, uint8_t delay);
/*
 * Configure le délai entre le moment ou le servo reçoit une
 * instruction et le moment où il répond.
 * Valeurs possibles : 0x00 -> 0xFE (254)
 * Unité : 2 microsecondes
 */

uint8_t AX_Configure_CW_Angle_Limit(AX *servo, uint16_t angle);
/*
 * Configure la limite d'angle dans le sens horaire (cf. doc)
 * Valeurs possibles : 0x000 -> 0x3FF (1023)
 */

uint8_t AX_Configure_CCW_Angle_Limit(AX *servo, uint16_t angle);
/*
 * Configure la limite d'angle dans le sens anti-horaire (cf. doc)
 * Valeurs possibles : 0x000 -> 0x3FF (1023)
 */

uint8_t AX_Configure_Control_Mode(AX *servo, AX_Mode mode);
/*
 * Configure le servomoteur en mode Joint ou Wheel.
 * 1 : Wheel mode, le servomoteur fixe une vitesse
 * 2 : Joint mode, le servomoteur fixe un angle
 * ATTENTION : CW_Angle_Limit et CCW_Angle_Limit doivent être
 * à 0 pour le mode Wheel, différents de 0 pour le mode Joint.
*/

uint8_t AX_Configure_Limit_Temperature(AX *servo, uint8_t temp);
/*
 * Configure la température max du servo avant détection d'une erreur
 * ERROR_OVER_HEATING.
 * Valeurs possibles : 0 -> 150
 * Cette fonction ne devrait pas exister.
 */

uint8_t AX_Configure_Lower_Limit_Voltage(AX *servo, uint8_t voltage);
/*
 * Configure la limite inférieure de tension avant détection d'une
 * erreur ERROR_INPUT_VOLTAGE.
 * Valeurs possibles : 50 -> 250 (0x32 -> 0x96)
 * Unité : 0.1 V
 */

uint8_t AX_Configure_Upper_Limit_Voltage(AX *servo, uint8_t voltage);
/*
 * Configure la limite supérieure de tension avant détection d'une
 * erreur ERROR_INPUT_VOLTAGE.
 * Valeurs possibles : 50 -> 250 (0x32 -> 0x96)
 * Unité : 0.1 V
 * Remarque : Les vrais mettront du triphasé
 */

uint8_t AX_Configure_Max_Torque(AX *servo, uint16_t max_torque);
/*
 * Configure le couple maximum du servomoteur.
 * Valeurs possibles : 0 -> 1023 (0% -> 100%)
 * Remarque : Cette fonction impose une limite mais
 * ne contrôle pas le couple, cf. Goal Torque (RAM).
 */

uint8_t AX_Configure_Return_Level(AX *servo, AX_Return_Level level);
/*
 * Configure le niveau de retour du servomoteur.
 * Valeurs possibles :
 * 0 : Aucun retour, sauf ping
 * 1 : Retour uniquement pour les commandes READ
 * 2 : Retour pour toutes les commandes
 * Remarque : Pour l'instant cette librairie ne fonctionne
 * correctement que dans le mode 1
*/

uint8_t AX_Configure_Alarm_Shutdown(AX *servo, AX_Alarm_Shutdown alarm);
/*
 * Configure les alarmes permettant de stopper le servomoteur en cas de problème.
 * Valeurs possibles : comibinaisons (OR) de
 * AX_ERROR_INPUT_VOLTAGE
 * AX_ERROR_OVER_HEATING
 * AX_ERROR_OVERLOAD
 * AX_ERROR_OVER_9000
 * ASTUCE DE HACKER : Incroyable ! Grâce à cette fonction, transformez
 * votre AX 320 en une superbe guirlande ! Vous n'en croirez pas vos
 * yeux !
 */

//======================================
//       COMMANDES SERVOMOTEUR       
//======================================

typedef enum AX_LED_Color_E{
  AX_LED_OFF = 0,
  AX_RED = 1,
  AX_GREEN = 2,
  AX_BLUE = 4,
  AX_YELLOW = AX_RED | AX_GREEN,
  AX_PINK = AX_RED | AX_BLUE,
  AX_CYAN = AX_GREEN | AX_BLUE,
  AX_WHITE = AX_RED | AX_GREEN | AX_BLUE
}AX_LED_Color;

uint8_t AX_Power_On(AX *servo, uint8_t now);
/*
 * Active le moteur.
 */

uint8_t AX_Power_Off(AX *servo, uint8_t now);
/*
 * Désactive le moteur.
 */

uint8_t AX_Set_LED(AX *servo, AX_LED_Color color, uint8_t now);
/*
 * Allume la LED du servo selon la couleur choisie.
 */

uint8_t AX_Set_D_Gain(AX *servo, uint8_t d_gain, uint8_t now);
/*
 * Modifie le gain en dérivée du régulateur PID.
 * Valeurs : 0 -> 254
 */

uint8_t AX_Set_I_Gain(AX *servo, uint8_t i_gain, uint8_t now);
/*
 * Modifie le gain en intégration du régulateur PID.
 * Valeurs : 0 -> 254
 */

uint8_t AX_Set_P_Gain(AX *servo, uint8_t p_gain, uint8_t now);
/*
 * Modifie le gain proportionnel du régulateur PID.
 * Valeurs : 0 -> 254
 */

uint8_t AX_Set_Goal_Position(AX *servo, uint16_t position, uint8_t now);
/*
 * Modifie la position cible du servo.
 * Valeurs possibles : 0 -> 1023 (0x3FF)
 * Unité : 0.29°
 * Remarque :
 * Cette valeur n'a un sens que si servo est en mode JOIN.
 * cf. doc
 */

typedef enum AX_Wheel_Direction_E{
  AX_CLOCKWISE, AX_COUNTERCLOCKWISE
}AX_Wheel_Direction;

uint8_t AX_Set_Goal_Speed_Join(AX *servo, uint16_t speed, uint8_t now);
/*
 * Modifie la vitesse cible du servo lorsqu'il est en mode Join
 * Valeurs possibles : 0-> 1023
 * Unité : 0.111 RPM
 * 0 : pas de contrôle de vitesse
 */

uint8_t AX_Set_Goal_Speed_Wheel(AX *servo, uint16_t speed, AX_Wheel_Direction dir, uint8_t now);
/*
 * Modifie la vitesse cible du servo lorsuqu'il est en mode Wheel
 * Valeurs possibles :
 * speed : 0 -> 1023, unité : 0.1% du max
 * dir : AX_CLOCKWISE ou AX_COUNTERCLOCKWISE
 * Remarque :
 * La vitesse n'est pas asservie, on règle la puissance
 * 1023*0.1% = 102,3%, bitches
 */

uint8_t AX_Set_Torque_Limit(AX *servo, uint16_t torque_limit, uint8_t now);
/*
 * Modifie la limite de couple.
 * Valeurs possibles : 0 -> 1023
 * Unité : environ 0.1%
 * Remarque :
 * En cas d'erreur matérielle (alarm shutdown),
 * cette valeur est mise à 0.
 */

uint8_t AX_Set_Punch(AX *servo, uint16_t punch, uint8_t now);
/*
 * Courant minimum pour contrôler le moteur
 * Valeurs possibles : 0x20 -> 0x3FF
 * Unité : inconnue
 * Remarque : la doc est pas claire sur ce point
 */

//======================================
//       LECTURE D'INFORMATIONS
//======================================
uint8_t AX_Get_Current_Position(AX *servo, uint16_t *position);
/*
 * Lis la position actuelle du servomoteur.
 * Valeurs : 0 (0°) -> 1023 (300°)
 * Unité : 0.29°
 */

uint8_t AX_Get_Current_Speed(AX *servo, AX_Wheel_Direction *direction, uint16_t *speed);
/*
 * Lis la vitesse du servomoteur et le sens de rotation.
 * Valeurs de vitesse : 0 -> 1023
 * Direction : AX_CLOCKWISE ou AX_COUNTERCLOCKWISE (cf. doc pour précisions).
 * Unité (Joint Mode) : 0.111 rpm
 * Unité (Wheel Mode) : environ 0.1% du maximum
 */

uint8_t AX_Get_Current_Load(AX *servo, AX_Wheel_Direction *direction, uint16_t *load);
/*
 * Lis une image du couple généré par le moteur.
 * Valeurs : 0 -> 1023
 * Direction : AX_CLOCKWISER ou AX_COUNTERCLOCKWISE (cf. doc pour précisions).
 * Unité : environ 0.1% du maximum
 */

uint8_t AX_Get_Current_Voltage(AX *servo, uint16_t *voltage);
/*
 * Lis le potentiel appliqué au servomoteur.
 * Unité : 0.1 V
 */

uint8_t AX_Get_Current_Temperature(AX *servo, uint16_t *temperature);
/*
 * Lis la température interne du servomoteur.
 * Unité : 1 °C.
 */

uint8_t AX_Is_Working(AX *servo, uint16_t *working);
/*
 * Lis le champs Torque Enable pour savoir si le servomoteur fonctionne.
 * Valeurs : 1 ou 0 respectivement si le moteur fonctionne ou non.
 */

uint8_t AX_Is_Moving(AX *servo, uint16_t *moving);
/*
 * Lis le champs Moving pour savoir si le servomoteur bouge.
 * Valeurs : 1 ou 0 respectivement si le moteur bouge ou non.
 */

uint8_t AX_Get_Hardware_Error(AX *servo, uint16_t *hw_error);
/*
 * Lis le champs Hardware Error Status du servomoteur.
 * Valeurs : Combinaison (OR) de l'énumération AX_Hardware_Error.
 */

/*
 * Remarque : Il y a plein d'autres champs qui peuvent être lus, mais
 * ils ne nécessitent pas de fonction dédiée (leur lecture est un cas
 * particulier d'utilisation). Pour les obtenir, il suffit d'utiliser
 * directement la fonction AX_Read.
 */

//==================================================
//              GESTION DES ERREURS
//==================================================
//Types d'erreurs
typedef enum AX_Error_Type_E{
  AX_ERR_INTERNAL = 0x0000, //Erreur interne à la librairie
  AX_ERR_LINK = 0x0100, //Erreur de communication (UART)
  AX_ERR_STATUS = 0x0200, //Erreur de traitement de l'instruction (AX)
  AX_ERR_HARDWARE = 0x0400, //Erreur matérielle (AX)
}AX_Error_Type;

//Erreurs internes à la librairie
typedef enum AX_Internal_Error_E{
  AX_ERR_ILLEGAL_ARGUMENTS, //Quand l'utilisateur en a trop pris
  AX_ERR_BUFFER_OVERFLOW, //Quand le buffer ne s'est pas fait respecter
}AX_Internal_Error;

//Erreurs de communication (UART)
typedef enum AX_Link_Error_E{
  AX_ERR_BAD_FRAME, //Quand le AX en a trop pris
  AX_ERR_TIMEOUT, //Quand on n'a pas eu le temps de se dépêcher
}AX_Link_Error;

//Erreurs d'instruction (AX)
typedef enum AX_Status_Error_E{
  AX_FAIL = 0x01,
  AX_BAD_INSTRUCTION = 0x02,
  AX_CRC_ERROR = 0x03,
  AX_DATA_RANGE_ERROR = 0x04,
  AX_DATA_LENGTH_ERROR = 0x05,
  AX_DATA_LIMIT_ERROR = 0x06,
  AX_ACCESS_ERROR = 0x07
}AX_Status_Error;

//Erreurs matérielles (AX)
typedef enum AX_Hardware_Error_E{
  AX_ERR_OVERLOAD = 0x01,
  AX_ERR_OVER_HEATING = 0x02,
  AX_ERR_INPUT_VOLTAGE = 0x04,
}AX_Hardware_Error;

//Macros sur les erreurs
#define AX_ERROR_TYPE(err) (AX_Error_Type) (err & 0xFF00)
#define AX_ERROR_CODE(err) (err & 0xFF)
#define AX_STATUS_ALERT(err) (AX_ERROR_CODE(err) >> 7)
#define AX_STATUS_ERROR(err) (AX_Status_Error) (AX_ERROR_CODE(err) & 0b0111111)
#define AX_INTERNAL_ERROR(err) (AX_Internal_Error) AX_ERROR_CODE(err)
#define AX_LINK_ERROR(err) (AX_Link_Error) AX_ERROR_CODE(err)
#define AX_HARDWARE_ERROR(err) (AX_Hardware_Error) AX_ERROR_CODE(err)

//Fonction de récupération de l'erreur
uint16_t AX_Get_Error();
/*
 * Retourne la dernière erreur détectée.  Le type d'erreur peut être
 * vérifié avec la macro AX_ERROR_TYPE().  Pour obtenir des
 * informations plus précises connaîssant le type d'erreur, il faut
 * utiliser les macros définies ci-dessus.
 */

uint8_t AX_Check_Status(AX *servo);
/*
 * Cette fonction vérifie le status d'un servomoteur après réception
 * d'un paquet.  Elle ne doit pas être appelée directement par
 * l'utilisateur, mais elle est utilisée dans les fonctions
 * d'utilisation du servomoteur. La fonction renvoie 1 en cas
 * d'erreur, 0 en l'absence d'erreurs.
 */

uint8_t AX_Check_Alert(AX *servo);
/*
 * Cette fonction vérifie l'état du bit d'alarme dans le dernier
 * paquet reçu par servo. Elle ne doit pas être appelée directement
 * par l'utilisateur. Si le bit d'alarme vaut 1, la fonction tente de
 * lire le champ Hardware Error Status du servomoteur pour récupérer
 * l'erreur. Elle renvoie 0 en l'absence d'erreur matérielle, 1 sinon.
 */

#endif
