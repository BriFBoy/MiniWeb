#include "../Include/content.h"
#include "../Include/global.h"
#include "../Include/http.h"
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

void serveConnection(const int clientfd) {
  char httprequest[MAXBUFFSIZE];
  httprequest[0] = '\0';
  char buff[MAXBUFFSIZE] = {0};
  Response response;
  int bytesread = 0;

  int n;
  while ((n = read(clientfd, buff, sizeof(buff) - NULL_TERMINATOR)) > 0) {
    buff[n] = '\0';
    bytesread += n;

    if (bytesread > MAXBUFFSIZE) {
      break;
    }
    strncat(httprequest, buff,
            sizeof(httprequest) - strlen(httprequest) - NULL_TERMINATOR);

    if (strstr(httprequest, "\r\n\r\n") != NULL) {
      break;
    }
  }
  memset(buff, 0, sizeof(buff));

  httpRequest *parsedRequest = parshttp(httprequest);

  // If parsing of http request is NULL,
  // send a INTERNAL_ERROR
  if (parsedRequest == NULL) {
    printf("Error parsing http\n");
    response.pResponse =
        getResponseFromError(INTERNAL_ERROR, &response.responseLenght);
    write(clientfd, response.pResponse, response.responseLenght);
  } else {
    printf("%s %s %s\n", parsedRequest->requestLine.method,
           parsedRequest->requestLine.path, parsedRequest->requestLine.version);
    fixNondirectpath(parsedRequest);

    sendResponse(clientfd, parsedRequest, &response);
    free(parsedRequest);
  }

  close(clientfd);
}

void sendResponse(const int clientfd, httpRequest *request,
                  Response *response) {
  enum statusCodes statuscode = SUCCESS;
  response->pBody = getContent(request->requestLine.path, &statuscode);

  if (response->pBody != NULL) {
    response->pResponse = malloc(MAXBUFFSIZE);
    snprintf(response->pResponse, MAXBUFFSIZE, "HTTP/1.0 200 OK\r\n\r\n%s",
             response->pBody);
    write(clientfd, response->pResponse, strlen(response->pResponse));

    free(response->pBody);
    free(response->pResponse);
  } else {
    response->pResponse =
        getResponseFromError(statuscode, &response->responseLenght);
    write(clientfd, response->pResponse, strlen(response->pResponse));

    free(response->pResponse);
  }
}
