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
      // puts (ep->d_name);
      char *temp_buffer = malloc(sizeof (char) *MAX_MESSAGE_LENGTH);
      temp_buffer = strcat(ep->d_name, "\n");
      write_to_socket(socket, strlen(temp_buffer), temp_buffer);
      // write_to_socket(socket, strlen("\n"), "\n");
    }

    (void) closedir (dp);
    return 0;
  }
  else
    return 1;
    // error("Couldn't open the directory");
}

// int send_directory_listing(int socket) {
//   char buffer[MAX_MESSAGE_LENGTH];
//   get_directory_listing(buffer);
//   return 0;
// }

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

      printf("SOMEONE CONNECTED\n");

      if(strcmp(read_from_socket(newsockfd), "-l") == 0) {
        
        if(send_directory_listing(newsockfd) == 0) {
          write_to_socket(newsockfd, strlen("200 SUCCESS"), "200 SUCCESS");
        } else {
          write_to_socket(newsockfd, strlen("FAILURE"), "FAILURE");
        }
        
      } else {
        // send_file(newsockfd, read_from_socket(newsockfd));
      }

    } else { // handle the parent fork
      close(newsockfd);
    }
  }

  return 0;
}