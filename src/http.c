#include "../Include/http.h"
#include "../Include/content.h"
#include "../Include/global.h"
#include "../Include/httpbuilder.h"
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

char *getResponseFromError(enum statusCodes statuscodes, int *responselenght) {
  *responselenght = 1024;
  char *pResponse = malloc(*responselenght);
  char *pbody;
  enum statusCodes statuscodes1;

  switch (statuscodes) {
  case INTERNAL_ERROR:

    addStatusLine(pResponse, "HTTP/1.0 500 INTERNAL SERVER ERROR",
                  *responselenght);
    break;
  case FILE_NOT_FOUND:

    pbody = getContent("/.errors/404.html", &statuscodes);
    if (pbody != NULL) {
      addStatusLine(pResponse, "HTTP/1.0 404 Not Found", *responselenght);
      addBody(pResponse, pbody, *responselenght);

      free(pbody);
    } else {
      addStatusLine(pResponse, "HTTP/1.0 404 Not Found\r\n", *responselenght);
    }
    break;
  case SUCCESS:
    break;
  }
  *responselenght = strlen(pResponse);
  return pResponse;
}
