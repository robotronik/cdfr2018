#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Socket TCP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

//Shared memory
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//Semaphore
#include <semaphore.h>

//Signal
#include <signal.h>

#define SHM_NAME "/score_cdfr_shm"
#define SEM_NAME "/score_cdfr_sem"

void handle_error(const char msg[]);
int init_sockaddr(const char IP[], const char port[], struct sockaddr_in *addr);
void sigterm_handler(int signo);

#endif
