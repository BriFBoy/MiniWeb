#include "../Include/content.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
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
  short port = 8080;
  char line[1024];
  struct sockaddr_in serveraddr;
  int clientfd;
  char buff[1024] = {0};
  char *pbody;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(port);

  int optval = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  if (bind(socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    printf("Error binding port\n");
    exit(1);
  }

  if (listen(socket_fd, 50) < 0) {
    printf("Error listening to socket\n");
    exit(1);
  }

  int n;
  for (;;) {
    printf("Waithing for connection...\n");
    clientfd = accept(socket_fd, NULL, NULL);

    while ((n = read(clientfd, line, sizeof(line) / sizeof(char))) > 0) {

      if (line[n - 1] == '\n')
        break;

      if (n <= 0)
        break;
    }

    pbody = getContent("/index.html");
    if (pbody != NULL) {
      snprintf(buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\n%s", pbody);
    } else {
      snprintf(buff, sizeof(buff),
               "HTTP/1.0 500 Internal Server Error\r\n\r\n");
    }
    write(clientfd, (char *)buff, strlen(buff));

    free(pbody);
    close(clientfd);
  }
  return EXIT_SUCCESS;
}
