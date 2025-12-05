#include "../Include/content.h"
#include "../Include/pars.h"
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

void serveConnection(const int clientfd);

int main(int argc, char *argv[]) {
  int socket_fd;
  short port = 8080;
  struct sockaddr_in serveraddr;
  int clientfd;

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

  for (;;) {
    printf("Waithing for connection...\n");
    clientfd = accept(socket_fd, NULL, NULL);
    serveConnection(clientfd);
  }

  return EXIT_SUCCESS;
}

void serveConnection(const int clientfd) {
  const int maxRequestLenghtInBytes = 5120;
  char buff[1024] = {0};
  char httprequest[maxRequestLenghtInBytes];
  httprequest[0] = '\0';
  char *pbody;
  int bytesread = 0;

  int n;
  while ((n = read(clientfd, buff, sizeof(buff) - 1)) > 0) {
    bytesread += n;
    if (bytesread > maxRequestLenghtInBytes) {
      continue;
    }
    printf("Bytes read: %d\n", bytesread);
    strcat(httprequest, buff);

    if (buff[n - 1] == '\n')
      break;
    if (n <= 0)
      break;
  }

  httpRequest *request = parshttp(buff);
  printf("%s", request->);
  memset(buff, 0, sizeof(buff));
  pbody = getContent("/index.html");
  if (pbody != NULL) {
    snprintf(buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\n%s", pbody);
  } else {
    snprintf(buff, sizeof(buff),
             "HTTP/1.0 500 Internal Server Error\r\n\r\nWe are haveing some "
             "problems right now. Please come back later");
  }
  write(clientfd, (char *)buff, strlen(buff));

  free(pbody);
  close(clientfd);
}
