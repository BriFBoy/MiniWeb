#include "../Include/configuration.h"
#include "../Include/dataStructure.h"
#include "../Include/global.h"
#include "../Include/http.h"
#include "../Include/logging.h"
#include "../Include/pars.h"
#include "../Include/read.h"
#include "../Include/sending.h"
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
#define PORT 8080

int serverSetup();
void serveConnection(const int clientfd);
void readIncommingData(char *buff, const int clientfd, char *httprequest,
                       enum statusCodes *status);

pthread_t *thread_pool;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;
static int shutdown_request = 0;

void *threadRunner(void *arg) {
  (void)arg;

  int client;
  while (!shutdown_request) {

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
void exit_program(int sig) {
  shutdown_request = 1;
  pthread_cond_broadcast(&cond_var);
  for (int i = 0; i < Config_getthread_amount(); i++) {
    pthread_join(thread_pool[i], NULL);
  }
  free(thread_pool);
  free(Config_getContentPath());
  exit(sig);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  char config_path[100] = {0};
  char *cwd = getcwd(NULL, 0);
  if (cwd) {
    snprintf(config_path, sizeof(config_path), "%s/%s", cwd, "config.json");
    free(cwd);
    load_config(config_path);
  } else {
    LOG_FATAL("Faild to get cwd. Unable to load config");
    exit(1);
  }

  thread_pool = malloc(sizeof(pthread_t) * Config_getthread_amount());
  int clientfd;
  int socket_fd = serverSetup();

  if (!thread_pool) {
    LOG_FATAL("Failed to malloc thread_pool");
    exit(1);
  }

  for (int i = 0; i < Config_getthread_amount(); i++) {
    pthread_create(&thread_pool[i], NULL, threadRunner, NULL);
  }

  signal(SIGINT, exit_program);
  struct timeval clientTimeout;
  clientTimeout.tv_sec = Config_getTimeout();
  clientTimeout.tv_usec = 0;

  LOG_INFO("Starting...");
  printf("Serving from: %s\n", Config_getContentPath());
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
  char *httprequest = calloc(MAX_REQUEST_SIZE, sizeof(char));
  char *buff = malloc(MAX_REQUEST_SIZE);
  enum statusCodes status;
  if (!buff || !httprequest) {
    LOG_ERROR("Faild to malloc mem to buffs");
    close(clientfd);
    return;
  }

  readIncommingData(buff, clientfd, httprequest, &status);
  if (status == REQUEST_TO_BIG) {
    LOG_WARN("Request became to big");
    send_error_message(clientfd, REQUEST_TO_BIG);
    free(httprequest);
    free(buff);
    return;
  }
  httpRequest *parsedRequest = parshttp(httprequest);

  if (!parsedRequest) {
    LOG_ERROR("Error parsing http");
    send_error_message(clientfd, PARSING_FAILED);

    free(httprequest);
    free(buff);
    free(parsedRequest);
    return;
  }

  // Only GET method Supported
  if (strcmp(parsedRequest->requestLine.method, "GET") != 0) {
    LOG_WARN("Unsupported method");
    send_error_message(clientfd, METHOD_NOT_ALLOWED);

    free(httprequest);
    free(buff);
    free(parsedRequest);
    return;
  } else {

    char log[200];
    snprintf(log, sizeof(log), "%s %s %s", parsedRequest->requestLine.method,
             parsedRequest->requestLine.path,
             parsedRequest->requestLine.version);
    LOG_INFO(log);

    // if the request is valid, find the correct file and send it
    fixNondirectpath(parsedRequest);
    sendResponse(clientfd, parsedRequest);
  }

  free(httprequest);
  free(buff);
  free(parsedRequest);
  close(clientfd);
}
