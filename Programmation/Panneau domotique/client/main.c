#include "main.h"

void handle_error(const char msg[]){
  perror(msg);
  exit(EXIT_FAILURE);
  do{}while(1);
}

/*
 * Parameters :
 * IP (v4)
 * Port number
 * Note that ports under 1024 need priviledged mode
 */
int main(int argc, char *argv[]){
  //Reading and verifying the arguments
  if(argc < 5){
    fprintf(stderr, "Too few arguments.\nclient <Host-IP> <Host-Port> <Server-IP> <Server-Port>\n");
    exit(EXIT_FAILURE);
  }

  //Preparing sockaddr
  struct sockaddr_in host_addr, server_addr;
  if(init_sockaddr(argv[1], argv[2], &host_addr) == -1){
    exit(EXIT_FAILURE);
  }

  if(init_sockaddr(argv[3], argv[4], &server_addr) == -1){
    exit(EXIT_FAILURE);
  }

  //Opening a TCP socket
  int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(tcp_socket == -1){
    handle_error("Could not create socket");
  }

  //Binding the socket to the given IP host_address and port
  if(bind(tcp_socket, (struct sockaddr*) &host_addr, sizeof(host_addr)) == -1){
    handle_error("Could not bind socket");
  }

  if(connect(tcp_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
    handle_error("Could not connect");
  }
  
  return EXIT_SUCCESS;
}

int init_sockaddr(const char IP[], const char port[], struct sockaddr_in *addr){
  addr->sin_family = AF_INET;
  addr->sin_port = htons(atoi(port));
  if(inet_aton(IP, &addr->sin_addr) == 0){
    fprintf(stderr, "%s is not a valid IP host_address.", IP);
    return -1;
  }
  return 0;
}
