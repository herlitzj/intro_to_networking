#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#define USAGE "chatclient [host name] [host port]"

// general error function
void error(const char *msg) {
  printf("Client: ");
  printf("%s\n", msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  
  if(argc < 3) {
    error(USAGE);
  }

  struct addrinfo message, *response;
  int sockfd, status;

  memset(&message, 0, sizeof(message));
  message.ai_family = AF_UNSPEC;
  message.ai_socktype = SOCK_STREAM;

  getaddrinfo(argv[1], argv[2], &message, &response);

  sockfd = socket(response->ai_family, response->ai_socktype, response->ai_protocol);

  status = connect(sockfd, response->ai_addr, response->ai_addrlen);

  if(status == -1) error("Error connecting to socket.");
  else write(sockfd, "Hello World!", 12);
}