#include "../Include/content.h"
#include "../Include/dataStructure.h"
#include "../Include/global.h"
#include "../Include/http.h"
#include "../Include/httpbuilder.h"
#include "../Include/pars.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <bits/pthreadtypes.h>
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

int serverSetup(int maxBacklog, short port);
void serveConnection(const int clientfd);
void sendResponse(const int clientfd, httpRequest *request, Response *response);
void readIncommingData(char *buff, int *bytesread, const int clientfd,
                       char *httprequest);

pthread_t thread_pool[20];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

void checkRunState() {
  if (getenv("MINIWEB_SOURCE") == NULL) {
    printf("Missing env MINIWEB_SOURCE\n");
    exit(EXIT_FAILURE);
  }
}

void *threadRunner(void *arg) {
  int client;
  while (true) {

    pthread_mutex_lock(&mutex);
    if ((client = dequeue()) == -1) {
      pthread_cond_wait(&cond_var, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    if (client != -1) {
      printf("serveing Connection\n");
      serveConnection(client);
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  checkRunState();

  int numThreads = 20;
  int maxBacklog = 500;
  short port = 8080;
  int clientfd;
  int socket_fd = serverSetup(maxBacklog, port);

  for (int i = 0; i < numThreads; i++) {
    pthread_create(&thread_pool[i], NULL, threadRunner, NULL);
    printf("Thead created\n");
  }

  printf("Hello World\n");
  struct timeval timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;
  for (;;) {

    fflush(stdout);
    clientfd = accept(socket_fd, NULL, NULL);
    if (clientfd <= 0)
      continue;
    pthread_mutex_lock(&mutex);
    enqueue(clientfd);
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex);
    printf("enqueued\n");
    setsockopt(clientfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
  }

  return EXIT_SUCCESS;
}

int serverSetup(int maxBacklog, short port) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  int optval = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  signal(SIGPIPE, SIG_IGN);

  struct sockaddr_in serveraddr = {0};
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    printf("Error binding port\n");
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, maxBacklog) < 0) {
    printf("Error listening to socket\n");
    exit(EXIT_FAILURE);
  }
  return socket_fd;
}

void serveConnection(const int clientfd) {
  char httprequest[MAXBUFFSIZE];
  memset(&httprequest, 0, sizeof(httprequest));
  httprequest[0] = '\0';
  char buff[MAXBUFFSIZE];
  Response response;
  int bytesread = 0;

  readIncommingData(buff, &bytesread, clientfd, httprequest);
  httpRequest *parsedRequest = parshttp(httprequest);

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

    response.pResponse = getResponseFromError(PARSING_FAILED, &body, &bodySize);
    write(clientfd, response.pResponse, strlen(response.pResponse));
  }

  close(clientfd);
}

void readIncommingData(char *buff, int *bytesread, const int clientfd,
                       char *httprequest) {
  httprequest[0] = '\0';
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
void sendResponse(const int clientfd, httpRequest *request,
                  Response *response) {
  enum statusCodes statuscode = SUCCESS;
  size_t bodySize;
  response->pBody =
      getContent(request->requestLine.path, &statuscode, &bodySize);

  if (response->pBody == NULL) {

    // sends the correct error
    response->pResponse =
        getResponseFromError(statuscode, &response->pBody, &bodySize);

    write(clientfd, response->pResponse, strlen(response->pResponse));
    write(clientfd, response->pBody, bodySize);

    free(response->pResponse);
    free(response->pBody);

    return;
  }

  response->pResponse = malloc(MAXBUFFSIZE);
  response->responseLenght = MAXBUFFSIZE;
  char content_lenght[100];

  createResponseHeader(response->pResponse, response->responseLenght,
                       "HTTP/1.0 200 OK", request->requestLine.path, bodySize);

  write(clientfd, response->pResponse, strlen(response->pResponse));
  write(clientfd, response->pBody, bodySize);

  free(response->pBody);
  free(response->pResponse);
}
