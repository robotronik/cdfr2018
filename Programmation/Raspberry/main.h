#ifndef __MAIN_H_
#define __MAIN_H_

#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "uart/robotronik_protocol.h"
#include "uart/remote_call.h"
#include "uart.h"
#include "logger.h"
#include "server.h"
#include "player.h"
#include "camera.h"

#define UART_PATH "/dev/serial0"
#define UART_SPEED B1000000

#define ID_ROBOT "1"
#define IP_ESP "192.168.0.2"
#define PORT_ESP "80"

#define PLAYER_PATH "player"
#define SONGS_PATH "../music/"
#define MATCH_SONG "../match_songs/gas.wav"
#define ERROR_SONG "../match_songs/sound_of_silence.wav"

#define CAMERA_PATH "camera.py"
#define VIDEO_PATH "../video/"
#define CAMERA_SOCKET "./camera_socket"

extern RP_Interface f4_iface;
extern RC_Server server;
extern volatile sig_atomic_t run;
extern volatile sig_atomic_t stop;

#endif
