#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#define USAGE "chatClient [host name] [host port]"
#define MAX_MESSAGE_LENGTH 500

// general error function
void error(const char *msg) {
  printf("Client: ");
  printf("%s\n", msg);
  exit(1);
}

// general function for reading data from the socket
char *read_from_socket(int socket) {
  int result;
  char *temp_buffer = malloc(sizeof (char) *MAX_MESSAGE_LENGTH);

  result = read(socket, temp_buffer, MAX_MESSAGE_LENGTH);
  if(result > 1) return temp_buffer;
  else error("Error reading from socket");
}

// general function for writing data to the socket
int write_to_socket(int socket, unsigned int message_length, void* message) {
  int result;

  result = write(socket, message, message_length);
  return result;
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
  write_to_socket(sockfd, 12, "Hello World!");
  printf("From the server: %s\n", read_from_socket(sockfd));
}