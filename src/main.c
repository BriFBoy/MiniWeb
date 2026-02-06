#include "../Include/configuration.h"
#include "../Include/content.h"
#include "../Include/dataStructure.h"
#include "../Include/global.h"
#include "../Include/http.h"
#include "../Include/httpbuilder.h"
#include "../Include/logging.h"
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

#define MAXBACKLOG 500
#define NUMTHREADS 20
#define PORT 8080

int serverSetup();
void serveConnection(const int clientfd);
void sendResponse(const int clientfd, const httpRequest *request,
                  Response *response);
void readIncommingData(char *buff, const int clientfd, char *httprequest,
                       enum statusCodes *status);

pthread_t thread_pool[20];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

void *threadRunner(void *arg) {
  int client;
  while (true) {

    pthread_mutex_lock(&mutex);
    if ((client = dequeue()) == -1) {
      pthread_cond_wait(&cond_var, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    if (client != -1) {
      serveConnection(client);
    }
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  char *cwd = getcwd(NULL, 0);
  char config_path[100];
  snprintf(config_path, sizeof(config_path), "%s/%s", cwd, "config.json");
  printf("%s", config_path);
  load_config(config_path);

  int clientfd;
  int socket_fd = serverSetup();

  for (int i = 0; i < NUMTHREADS; i++) {
    pthread_create(&thread_pool[i], NULL, threadRunner, NULL);
  }

  struct timeval clientTimeout;
  clientTimeout.tv_sec = Config_getTimeout();
  clientTimeout.tv_usec = 0;

  LOG_INFO("Starting...");
  for (;;) {

    fflush(stdout);
    clientfd = accept(socket_fd, NULL, NULL);
    if (clientfd <= 0)
      continue;

    pthread_mutex_lock(&mutex);
    enqueue(clientfd);
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex);

    setsockopt(clientfd, SOL_SOCKET, SO_RCVTIMEO, &clientTimeout,
               sizeof(clientTimeout));
  }

  return EXIT_SUCCESS;
}

int serverSetup() {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  int optval = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  signal(SIGPIPE, SIG_IGN);

  struct sockaddr_in serveraddr = {0};
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(Config_getPort());

  if (bind(socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    LOG_FATAL("Error binding port");
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, MAXBACKLOG) < 0) {
    LOG_FATAL("Error listening to port");
    exit(EXIT_FAILURE);
  }
  return socket_fd;
}

void serveConnection(const int clientfd) {
  const int MAX_REQUEST_SIZE = Config_getMaxRequestSize();
  char httprequest[MAX_REQUEST_SIZE];
  memset(&httprequest, 0, sizeof(httprequest));
  httprequest[0] = '\0';
  char buff[MAX_REQUEST_SIZE];
  Response response;
  enum statusCodes status;

  readIncommingData(buff, clientfd, httprequest, &status);
  if (status == REQUEST_TO_BIG) {
    LOG_WARN("Request became to big");
    unsigned char *body;
    size_t bodySize;

    response.pResponse = getResponseFromError(REQUEST_TO_BIG, &body, &bodySize);
    write(clientfd, response.pResponse, strlen(response.pResponse));
    close(clientfd);
    return;
  }
  httpRequest *parsedRequest = parshttp(httprequest);

  if (!parsedRequest) {
    LOG_ERROR("Error parsing http");
    unsigned char *body;
    size_t bodySize;

    response.pResponse = getResponseFromError(PARSING_FAILED, &body, &bodySize);
    write(clientfd, response.pResponse, strlen(response.pResponse));
    close(clientfd);
    return;
  }

  // Only GET method Supported
  if (strcmp(parsedRequest->requestLine.method, "GET") != 0) {
    LOG_WARN("Unsupported method");
    unsigned char *body;
    size_t bodySize;

    response.pResponse =
        getResponseFromError(METHOD_NOT_ALLOWED, &body, &bodySize);
    write(clientfd, response.pResponse, strlen(response.pResponse));

    free(parsedRequest);
    close(clientfd);
    return;
  } else {

    char log[200];
    snprintf(log, sizeof(log), "%s %s %s", parsedRequest->requestLine.method,
             parsedRequest->requestLine.path,
             parsedRequest->requestLine.version);
    LOG_INFO(log);

    // if the request is valid, find the correct file and send it
    fixNondirectpath(parsedRequest);
    sendResponse(clientfd, parsedRequest, &response);
  }

  free(parsedRequest);
  close(clientfd);
}

void readIncommingData(char *buff, const int clientfd, char *httprequest,
                       enum statusCodes *status) {
  int bytesread = 0;
  const int MAX_REQUEST_SIZE = Config_getMaxRequestSize();
  httprequest[0] = '\0';
  int n;

  if (MAX_REQUEST_SIZE - NULL_TERMINATOR <= 0) {
    *status = REQUEST_TO_BIG;
    return;
  }
  while ((n = read(clientfd, buff, MAX_REQUEST_SIZE - NULL_TERMINATOR)) > 0) {
    bytesread += n;

    if (bytesread > MAX_REQUEST_SIZE) {
      *status = REQUEST_TO_BIG;
      break;
    }

    buff[n] = '\0';
    strncat(httprequest, buff,
            MAX_REQUEST_SIZE - strlen(httprequest) - NULL_TERMINATOR);

    if (strstr(httprequest, "\r\n\r\n") != NULL) {
      break;
    }
  }
}
void sendResponse(const int clientfd, const httpRequest *request,
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

  createResponseHeader(response->pResponse, response->responseLenght,
                       "HTTP/1.0 200 OK", request->requestLine.path, bodySize);

  write(clientfd, response->pResponse, strlen(response->pResponse));
  write(clientfd, response->pBody, bodySize);

  free(response->pBody);
  free(response->pResponse);
}
