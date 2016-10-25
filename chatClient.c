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

  result = recv(socket, temp_buffer, MAX_MESSAGE_LENGTH, 0);
  printf("BUFFER CONTENT: %s, MESSAGE LENGTH: %d\n", temp_buffer, result);
  if(result > 1) return temp_buffer;
  else if(result == 0) error("Connection closed by server");
  else error("Error reading from socket");
}

// general function for writing data to the socket
int write_to_socket(int socket, unsigned int message_length, void* message) {
  int result;

  result = send(socket, message, message_length, 0);
  return result;
}

// function to get message and length from user
void get_message_from_user(char *buff) {
  printf("> ");
  fgets(buff, MAX_MESSAGE_LENGTH, stdin);
  buff[strlen(buff)-1] = '\0';
}

void get_handle(char *buff) {
  size_t size = MAX_MESSAGE_LENGTH;
  printf("What is your handle: ");
  fgets(buff, size, stdin);
  printf("TEST\n");
}

int main(int argc, char *argv[])
{
  char *message_to_send = malloc(MAX_MESSAGE_LENGTH + 1);
  char *handle = malloc(MAX_MESSAGE_LENGTH + 1);
  unsigned int message_length;
  
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

  while(1) {
    char *reply_from_server = malloc(MAX_MESSAGE_LENGTH + 1);
    if(strlen(handle) == 0) get_handle(handle);
    get_message_from_user(message_to_send);
    message_length = strlen(message_to_send);
    printf("MESSAGE TO SEND %s: %d\n", message_to_send, message_length);
    write_to_socket(sockfd, message_length, message_to_send);
    reply_from_server = read_from_socket(sockfd);
    printf("From the server: %s\n", reply_from_server);
    fflush(stdout);  
  }
}