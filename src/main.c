#include "../Include/content.h"
#include "../Include/global.h"
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
void fixNondirectpath(httpRequest *request);
char *getResponseFromError(enum statusCodes statuscodes, int *lenght);

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

    fflush(stdout);
    clientfd = accept(socket_fd, NULL, NULL);
    serveConnection(clientfd);
  }

  return EXIT_SUCCESS;
}

void serveConnection(const int clientfd) {
  const int maxRequestLenghtInBytes = MAXBUFFSIZE;
  char buff[MAXBUFFSIZE] = {0};
  char httprequest[maxRequestLenghtInBytes];
  httprequest[0] = '\0';
  char *pbody;
  int bytesread = 0;

  int n;
  while ((n = read(clientfd, buff, sizeof(buff) - 1)) > 0) {
    buff[n] = '\0';
    bytesread += n;

    if (bytesread > maxRequestLenghtInBytes) {
      break;
    }
    strncat(httprequest, buff, sizeof(httprequest) - strlen(httprequest) - 1);

    if (strstr(httprequest, "\r\n\r\n") != NULL) {
      break;
    }
  }
  printf("Bytes read: %d\n", bytesread);

  httpRequest *request = parshttp(httprequest);
  char *response;
  int responselenght;
  if (request == NULL) {
    response = getResponseFromError(INTERNAL_ERROR, &responselenght);
    write(clientfd, response, responselenght);
  } else {

    printf("%s %s %s\n", request->requestLine.method, request->requestLine.url,
           request->requestLine.version);
    memset(buff, 0, sizeof(buff));

    fixNondirectpath(request);

    enum statusCodes statuscode = SUCCESS;
    pbody = getContent(request->requestLine.url, &statuscode);
    if (pbody != NULL) {
      snprintf(buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\n%s", pbody);
      write(clientfd, (char *)buff, strlen(buff));
      free(pbody);
    } else {
      response = getResponseFromError(statuscode, &responselenght);
      write(clientfd, response, strlen(response));

      free(response);
    }
    free(request);
  }

  close(clientfd);
}

void fixNondirectpath(httpRequest *request) {
  char path[100];
  strncpy(path, request->requestLine.url, sizeof(path));

  if (strchr(request->requestLine.url, '.') == NULL) {
    if (path[strlen(path) - 1] == '/') {
      strncat(request->requestLine.url, "index.html",
              sizeof(request->requestLine.url) -
                  strlen(request->requestLine.url) - 1);
    } else {
      strncat(request->requestLine.url, "/index.html",
              sizeof(request->requestLine.url) -
                  strlen(request->requestLine.url) - 1);
    }
  }
}

char *getResponseFromError(enum statusCodes statuscodes, int *responselenght) {
  *responselenght = 1024;
  char *pResponse = malloc(*responselenght);
  char *pbody;
  enum statusCodes statuscodes1;

  switch (statuscodes) {
  case INTERNAL_ERROR:

    strncpy(pResponse, "HTTP/1.0 500 Internal Server Error\r\n\r\n",
            *responselenght);
    break;
  case FILE_NOT_FOUND:

    pbody = getContent("/.errors/404.html", &statuscodes);
    if (pbody != NULL) {
      snprintf(pResponse, *responselenght, "HTTP/1.0 404 Not Found\r\n\r\n%s",
               pbody);
      free(pbody);
    } else {
      strncpy(pResponse, "HTTP/1.0 404 Not Found\r\n\r\n", *responselenght);
    }
    break;
  case SUCCESS:
    break;
  }
  *responselenght = strlen(pResponse);
  return pResponse;
}
