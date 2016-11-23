#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#define USAGE "ftServer [server port]"
#define MAX_MESSAGE_LENGTH 500
#define BACKLOG 10

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
}

int main(int argc, char *argv[]) {
  pid_t pid, wpid;
  int sockfd, newsockfd, portno, status;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  if (argc < 2) {
    error(USAGE);
  }


  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("Error opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    error("Error on binding");

  status = listen(sockfd, BACKLOG);
  
  if(status == -1) error("Error listening for connections.");
  printf("Waiting for connections...\n");
  
  while(1) {
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");


    if((pid = fork()) < 0) { // handle and error forking
      error("Error forking child process");
    } else if (pid == 0) { // handle the child fork

      // deal with connection requests
      
    } else { // handle the parent fork
      close(newsockfd);
    }
  }

  return 0;
}