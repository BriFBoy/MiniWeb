#include "../Include/pars.h"
#include "../Include/http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum status { METADATA, HEADER, BODY };

int tokenizeStatusLine(char *psavestat2, httpRequest *request) {
  char *pstr;
  pstr = strtok_r(psavestat2, " ", &psavestat2);
  if (!pstr)
    return PARSING_FAILED;
  strncpy(request->requestLine.method, pstr,
          sizeof(request->requestLine.method) - NULL_TERMINATOR);
  request->requestLine.method[sizeof(request->requestLine.method) - 1] = '\0';

  pstr = strtok_r(NULL, " ", &psavestat2);
  if (!pstr)
    return PARSING_FAILED;
  strncpy(request->requestLine.path, pstr,
          sizeof(request->requestLine.path) - NULL_TERMINATOR);
  request->requestLine.path[sizeof(request->requestLine.path) - 1] = '\0';

  pstr = strtok_r(NULL, " ", &psavestat2);
  if (!pstr)
    return PARSING_FAILED;
  strncpy(request->requestLine.version, pstr,
          sizeof(request->requestLine.version) - NULL_TERMINATOR);
  strtrim(request->requestLine.version);
  request->requestLine.version[sizeof(request->requestLine.version) - 1] = '\0';

  return SUCCESS;
}

int tokenizeHeaderLine(char *pline, httpRequest *request, int *headerlength) {
  char *pstr;

  pstr = strtok_r(pline, ":", &pline);
  if (pstr == NULL)
    return PARSING_FAILED;
  strncpy(request->header[*headerlength].key, pstr,
          sizeof(request->header->key) - NULL_TERMINATOR);

  pstr = strtok_r(NULL, "", &pline);
  if (pstr == NULL)
    return PARSING_FAILED;
  strncpy(request->header[*headerlength].value, pstr,
          sizeof(request->header->value) - NULL_TERMINATOR);
  strtrim(request->header[*headerlength].value);

  headerlength++;

  return SUCCESS;
}

httpRequest *parshttp(char *httprequest) {
  int status = METADATA;
  httpRequest *request = calloc(1, sizeof(httpRequest));
  int headerlength = 0;
  char *psavestat1 = httprequest;
  char *pline = strtok_r(httprequest, "\r\n", &psavestat1);
  int headerMaxLenght = sizeof(request->header) / sizeof(request->header[0]);

  do {
    if (!pline)
      break;

    switch (status) {
    case METADATA: {

      tokenizeStatusLine(pline, request);
      status = HEADER;
      break;
    }
    case HEADER:
      while ((pline = strtok_r(NULL, "\r\n", &psavestat1)) != NULL &&
             headerlength <= headerMaxLenght) {
        tokenizeHeaderLine(pline, request, &headerlength);
      }
      request->headerlenght = headerlength;
      break;
    case BODY:
      // Body not suported
      break;
    }
  } while ((pline = strtok_r(NULL, "\r\n", &psavestat1)) != NULL);
  return request;
}

void strtrim(char *str) {
  int len = strlen(str);
  int start = 0, end = len - 1;

  while (str[start] == ' ' || str[start] == '\r' || str[start] == '\n') {
    start++;
  }

  while (str[end] == ' ' || str[end] == '\r' || str[end] == '\n') {
    end--;
  }

  if ((start > 0 || end < (len - 1)) && end > start) {
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
  }
}
