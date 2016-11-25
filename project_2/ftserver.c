#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>
#include <netdb.h> 
#define USAGE "ftServer [server port]"
#define MAX_MESSAGE_LENGTH 2048
#define BACKLOG 10
#define SUCCESS "200 SUCCESS"
#define FILE_NOT_FOUND "400 FILE NOT FOUND"
#define INTERNAL_ERROR "500 INTERNAL ERROR"

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

int send_directory_listing(int socket) {
  DIR *dp;
  struct dirent *ep;     
  dp = opendir ("./");

  if (dp != NULL)
  {
    while (ep = readdir (dp)) {
      char *temp_buffer = malloc(sizeof (char) *MAX_MESSAGE_LENGTH);
      temp_buffer = strcat(ep->d_name, "\n");
      write_to_socket(socket, strlen(temp_buffer), temp_buffer);
    }

    (void) closedir (dp);
    return 0;
  }
  else
    return 1;
}

int send_file(int socket, char *file_name) {
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(file_name, "r");
  if (fp == NULL)
    return 1;

  while ((read = getline(&line, &len, fp)) != -1) {
    printf("Retrieved line of length %zu :\n", read);
    write_to_socket(socket, strlen(line), line);
  }

  fclose(fp);
  return 0;
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
  printf("Server open on %d\n", portno);
  
  while(1) {
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");


    if((pid = fork()) < 0) { // handle process and error forking
      error("Error forking child process");
    } else if (pid == 0) { // handle the child fork

      if(strcmp(read_from_socket(newsockfd), "-l") == 0) {

        printf("Sending directory listing...\n");
        
        if(send_directory_listing(newsockfd) == 0) {
          write_to_socket(newsockfd, strlen(SUCCESS), SUCCESS);
        } else {
          write_to_socket(newsockfd, strlen(INTERNAL_ERROR), INTERNAL_ERROR);
        }

      } else {

        printf("Sending file...\n");

        write_to_socket(newsockfd, strlen(SUCCESS), SUCCESS);
        char *file_and_port = read_from_socket(newsockfd);
        char *file_name = strtok(file_and_port, "|");
        char *data_port = strtok(NULL, "|");
        printf("file: %s\n", file_name);
        printf("port: %s\n", data_port);

        int send_status = send_file(newsockfd, file_name);

        if(send_status == 0) {
          write_to_socket(newsockfd, strlen(SUCCESS), SUCCESS);
        } else if(send_status == 1) {
          write_to_socket(newsockfd, strlen(FILE_NOT_FOUND), FILE_NOT_FOUND);
        }

      }

    } else { // handle the parent fork
      close(newsockfd);
    }
  }

  return 0;
}