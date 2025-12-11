#include "../Include/http.h"
#include "../Include/content.h"
#include "../Include/global.h"
#include "../Include/httpbuilder.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fixNondirectpath(httpRequest *request) {
  char *path = malloc(strlen(request->requestLine.path) + 1);
  strncpy(path, request->requestLine.path,
          strlen(request->requestLine.path) + NULL_TERMINATOR);

  if (strchr(request->requestLine.path, '.') == NULL) {
    if (path[strlen(path) - NULL_TERMINATOR] == '/') {
      strncat(request->requestLine.path, "index.html",
              sizeof(request->requestLine.path) -
                  strlen(request->requestLine.path) - NULL_TERMINATOR);
    } else {
      strncat(request->requestLine.path, "/index.html",
              sizeof(request->requestLine.path) -
                  strlen(request->requestLine.path) - NULL_TERMINATOR);
    }
  }
  free(path);
}

char *getResponseFromError(enum statusCodes statuscodes, unsigned char **pbody,
                           size_t *bodySize) {
  char *pResponse = malloc(MAXBUFFSIZE);
  if (pResponse == NULL) {
    printf("Unable to malloc in getResponseFromError");
    return NULL;
  }
  pResponse[0] = '\0';

  switch (statuscodes) {
  case INTERNAL_ERROR:

    addStatusLine(pResponse, "HTTP/1.0 500 INTERNAL SERVER ERROR\r\n",
                  MAXBUFFSIZE);
    break;
  case FILE_NOT_FOUND:

    *pbody = getContent("/.errors/404.html", &statuscodes, bodySize);
    if (pbody != NULL) {
      createResponse(pResponse, MAXBUFFSIZE, "HTTP/1.0 404 Not Found",
                     "/.errors/404.html", *bodySize);

    } else {
      addStatusLine(pResponse, "HTTP/1.0 404 Not Found\r\n", MAXBUFFSIZE);
    }
    break;
  case SUCCESS:
    break;
  }
  return pResponse;
}
