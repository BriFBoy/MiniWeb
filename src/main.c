#include "../Include/content.h"
#include "../Include/global.h"
#include "../Include/http.h"
#include "../Include/httpbuilder.h"
#include "../Include/pars.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

void serveConnection(const int clientfd);
void sendResponse(const int clientfd, httpRequest *request, Response *response);

void checkRunState() {
  if (getenv("MINIWEB_SOURCE") == NULL) {
    printf("Missing env MINIWEB_SOURCE\n");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  checkRunState();

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
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, 50) < 0) {
    printf("Error listening to socket\n");
    exit(EXIT_FAILURE);
  }

  for (;;) {

    fflush(stdout);
    clientfd = accept(socket_fd, NULL, NULL);
    serveConnection(clientfd);
  }

  return EXIT_SUCCESS;
}

void readIncommingData(char *buff, int *bytesread, const int clientfd,
                       char *httprequest) {
  int n;
  while ((n = read(clientfd, buff, MAXBUFFSIZE - NULL_TERMINATOR)) > 0) {
    buff[n] = '\0';
    *bytesread += n;

    if (*bytesread > MAXBUFFSIZE) {
      break;
    }
    strncat(httprequest, buff,
            MAXBUFFSIZE - strlen(httprequest) - NULL_TERMINATOR);

    if (strstr(httprequest, "\r\n\r\n") != NULL) {
      break;
    }
  }
}

void serveConnection(const int clientfd) {
  char *httprequest = malloc(MAXBUFFSIZE);
  httprequest[0] = '\0';
  char *buff = malloc(MAXBUFFSIZE);
  Response response;
  int bytesread = 0;

  readIncommingData(buff, &bytesread, clientfd, httprequest);
  httpRequest *parsedRequest = parshttp(httprequest);

  free(buff);
  free(httprequest);

  if (parsedRequest) {
    printf("%s %s %s\n", parsedRequest->requestLine.method,
           parsedRequest->requestLine.path, parsedRequest->requestLine.version);

    fixNondirectpath(parsedRequest);
    sendResponse(clientfd, parsedRequest, &response);

    free(parsedRequest);
  } else {
    printf("Error parsing http\n");
    unsigned char *body;
    size_t bodySize;

    response.pResponse = getResponseFromError(INTERNAL_ERROR, &body, &bodySize);
    write(clientfd, response.pResponse, response.responseLenght);
  }

  close(clientfd);
}

void sendResponse(const int clientfd, httpRequest *request,
                  Response *response) {
  enum statusCodes statuscode = SUCCESS;
  size_t bodySize;
  response->pBody =
      getContent(request->requestLine.path, &statuscode, &bodySize);

  if (response->pBody != NULL) {
    response->pResponse = malloc(MAXBUFFSIZE);
    response->responseLenght = MAXBUFFSIZE;
    char content_lenght[100];

    createResponse(response->pResponse, response->responseLenght,
                   "HTTP/1.0 200 OK", request->requestLine.path, bodySize);

    write(clientfd, response->pResponse, strlen(response->pResponse));
    write(clientfd, response->pBody, bodySize);

    free(response->pBody);
    free(response->pResponse);
  } else {
    response->pResponse =
        getResponseFromError(statuscode, &response->pBody, &bodySize);

    write(clientfd, response->pResponse, strlen(response->pResponse));
    write(clientfd, response->pBody, bodySize);
    if (response->pBody != NULL) {
      printf("body is NULL\n");
    }

    free(response->pResponse);
    free(response->pBody);
  }
}
