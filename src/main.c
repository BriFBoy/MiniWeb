#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int socket_fd;
  short port = 8081;
  char line[1024];
  struct sockaddr_in serveraddr;
  int clientfd;
  char buff[1024] = {0};

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    printf("error binding port");
    exit(1);
  }
  if (listen(socket_fd, 50) < 0) {
    printf("error listening to socket");
    exit(1);
  }
  int n;
  while (true) {
    printf("Waithing for connection");
    clientfd = accept(socket_fd, NULL, NULL);

    while ((n = read(clientfd, line, sizeof(line) / sizeof(char))) > 0) {

      printf("%s", line);

      if (line[strlen(line) - 1] == '\n') {
        break;
      }
      if (n < 0) {
        break;
      }
    }
    snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");
    write(clientfd, (char *)buff, strlen(buff));
    close(clientfd);
  }
  return EXIT_SUCCESS;
}
