#include "../Include/content.h"
#include "../Include/http.h"
#include "../Include/httpbuilder.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void send_error_message(const int clientfd, const enum statusCodes error) {
  Response response;
  size_t bodySize = 0;

  response.pResponse = getResponseFromError(error, &response.pBody, &bodySize);

  write(clientfd, response.pResponse, strlen(response.pResponse));
  if (response.pBody) {
    write(clientfd, response.pBody, bodySize);
    free(response.pBody);
  }
  free(response.pResponse);
  close(clientfd);
}

void sendResponse(const int clientfd, const httpRequest *request) {
  Response response;
  enum statusCodes statuscode = SUCCESS;
  size_t bodySize;

  response.pBody =
      getContent(request->requestLine.path, &statuscode, &bodySize);

  if (response.pBody == NULL) {

    send_error_message(clientfd, statuscode);

    return;
  }

  response.pResponse = malloc(MAXBUFFSIZE);
  response.responseLenght = MAXBUFFSIZE;

  createResponseHeader(response.pResponse, response.responseLenght,
                       "HTTP/1.0 200 OK", request->requestLine.path, bodySize);

  write(clientfd, response.pResponse, strlen(response.pResponse));
  write(clientfd, response.pBody, bodySize);

  free(response.pBody);
  free(response.pResponse);
}
