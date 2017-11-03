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

typedef struct XL_320_Instruction_Packet_S{
  uint8_t id;
  XL_320_Instruction instruction;
  uint8_t nb_params;
  uint8_t *params;
}XL_320_Instruction_Packet;

typedef struct XL_320_Status_Packet_S{
  uint8_t id;
  uint8_t error;
  uint8_t nb_params;
  uint8_t *params;
}XL_320_Status_Packet;

uint8_t build_frame(XL_320_Instruction_Packet *packet, uint8_t buffer[XL_320_BUFFER_SIZE]);
/*
 * Construit une trame à partir d'un paquet instruction et l'écrit dans le buffer.
 * La fonction retourne la taille effective de la trame construite en cas de succès, 0 en cas d'erreur.
 */

//Contrôle de redondance cyclique
uint16_t update_crc(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

#endif
