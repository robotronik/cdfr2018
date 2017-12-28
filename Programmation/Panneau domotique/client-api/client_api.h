#ifndef CLIENT_API_H
#define CLIENT_API_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

//Shared memory
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Semaphore
#include <semaphore.h>

//Signal
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SC_SHM_NAME "/score_cdfr_shm"
#define SC_SEM_NAME "/score_cdfr_sem"
#define SC_CLIENT_PATH "../client/client"
#define SC_SERVER_IP "192.168.0.3"
#define SC_SERVER_PORT 80

typedef struct Scoreboard_Client_S{
  int shm_fd;
  uint16_t *score;
  sem_t *sem;
  pid_t pid;
}Scoreboard_Client;
/*
 * This structure store the data of the client.  Note that only one
 * client should be created and started for the whole operating
 * system, otherwise their behavior would be indeterminate.
 */

extern sig_atomic_t sc_running;
extern Scoreboard_Client sc_client;
uint16_t* SC_Start(const char *id, const char* server_ip, const char* server_port);
/*
 * Start a scoreboard client as a child process and initializes the
 * client's struct.  On success, it returns a pointer on the variable
 * that will be used to store the score. On error, the function return
 * a NULL pointer.
 */

void SC_Stop();
/*
 * Send a SIGTERM signal to the scoreboard client, wait for it to
 * stop, then free the resources previously allocated in the client's
 * struct. After that, the variable returned by SC_Start can't be used
 * anymore to avoid indeterminate behavior.
 */

int SC_Update();
/*
 * Tells the client that the score has been updated and is ready to be
 * sent.
 */

void SC_SIGCHLD_Handler(int signo);

#endif
