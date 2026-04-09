#include "../Include/http.h"
#include "../Include/configuration.h"
#include "../Include/content.h"
#include "../Include/global.h"
#include "../Include/httpbuilder.h"
#include "../Include/logging.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pair G_MIME[] = {{".html", "text/html"},     {".css", "text/css"},
                 {".js", "text/javascript"}, {".ico", "image/x-icon"},
                 {".png", "image/png"},      {".jpg", "image/jpg"},
                 {".jpeg", "image/jpeg"}};

const int G_MIME_COUNT = sizeof(G_MIME) / sizeof(G_MIME[0]);

void fixNondirectpath(httpRequest *request) {
  char *path = malloc(strlen(request->requestLine.path) + 1);
  strncpy(path, request->requestLine.path,
          strlen(request->requestLine.path) + NULL_TERMINATOR);

  if (strchr(request->requestLine.path, '.') == NULL) {
    if (path[strlen(path) - NULL_TERMINATOR] == '/') {
      strncat(request->requestLine.path, Config_getRootFile(),
              sizeof(request->requestLine.path) -
                  strlen(request->requestLine.path) - NULL_TERMINATOR);
    } else {
      strncat(request->requestLine.path, "/",
              sizeof(request->requestLine.path) -
                  strlen(request->requestLine.path) - NULL_TERMINATOR);
      strncat(request->requestLine.path, Config_getRootFile(),
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
    LOG_FATAL("Unable to Malloc");
    return NULL;
  }
  pResponse[0] = '\0';

  switch (statuscodes) {
  case INTERNAL_ERROR:

    addStatusLine(pResponse, "HTTP/1.0 500 Internal Server Error\r\n",
                  MAXBUFFSIZE);
    break;
  case FILE_NOT_FOUND:
    *pbody = getContent(Config_getError(404), &statuscodes, bodySize);
    if (pbody != NULL) {
      createResponseHeader(pResponse, MAXBUFFSIZE, "HTTP/1.0 404 Not Found",
                           "/.errors/404.html", *bodySize);

    } else {
      addStatusLine(pResponse, "HTTP/1.0 404 Not Found\r\n", MAXBUFFSIZE);
    }

    break;
  case SUCCESS:
    break;
  case PARSING_FAILED:
    *pbody = getContent(Config_getError(500), &statuscodes, bodySize);
    if (pbody == NULL && bodySize <= 0) {
      addStatusLine(pResponse, "HTTP/1.0 500 Internal Server Error\r\n",
                    MAXBUFFSIZE);
    }
    createResponseHeader(pResponse, MAXBUFFSIZE,
                         "HTTP/1.0 500 Internal Server Error",
                         "/.errors/500.html", *bodySize);

    break;
  case METHOD_NOT_ALLOWED:
    *pbody = getContent(Config_getError(405), &statuscodes, bodySize);
    if (pbody == NULL || bodySize <= 0) {
      addStatusLine(pResponse, "HTTP/1.0 405 Method Not Allowed\r\n",
                    MAXBUFFSIZE);
    } else {
      createResponseHeader(pResponse, MAXBUFFSIZE,
                           "HTTP/1.0 405 Method Not Allowed ",
                           "/.errors/500.html", *bodySize);
    }
    break;
  case REQUEST_TO_BIG:
    *pbody = getContent(Config_getError(413), &statuscodes, bodySize);
    if (pbody == NULL || bodySize <= 0) {
      addStatusLine(pResponse, "HTTP/1.0 413 Payload Too Large \r\n",
                    MAXBUFFSIZE);
    } else {

      createResponseHeader(pResponse, MAXBUFFSIZE,
                           "HTTP/1.0 413 Payload Too Large",
                           "/.errors/500.html", *bodySize);
    }
    break;
  }
  return pResponse;
}
