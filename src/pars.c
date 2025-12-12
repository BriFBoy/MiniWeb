#include "../Include/pars.h"
#include "../Include/http.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum status { METADATA, HEADER, BODY };

httpRequest *parshttp(char *httprequest) {
  char *pstr;
  int status = METADATA;
  httpRequest *request = malloc(sizeof(httpRequest));
  char *psavestat1 = httprequest;
  char *pline;
  char *psavestat2;
  int requestlenght = 0;

  pline = strtok_r(httprequest, "\r\n", &psavestat1);
  do {
    if (pline == NULL) {
      break;
    }
    psavestat2 = pline;

    switch (status) {
    case METADATA: {
      pstr = strtok_r(psavestat2, " ", &psavestat2);
      if (pstr != NULL) {
        strncpy(request->requestLine.method, pstr, strlen(pstr) + 1);
      } else {
        return NULL;
      }
      pstr = strtok_r(NULL, " ", &psavestat2);
      if (pstr != NULL) {
        strncpy(request->requestLine.path, pstr, strlen(pstr) + 1);
      } else {
        return NULL;
      }
      pstr = strtok_r(NULL, " ", &psavestat2);
      if (pstr != NULL) {
        strncpy(request->requestLine.version, pstr, strlen(pstr) + 1);
        strtrim(request->requestLine.version);
      } else {
        return NULL;
      }

      status = HEADER;
      break;
    }
    case HEADER:
      while ((pline = strtok_r(NULL, "\r\n", &psavestat1)) != NULL) {

        pstr = strtok_r(pline, ":", &psavestat2);
        if (pstr == NULL)
          break;
        strncpy(request->header[requestlenght].key, pstr, strlen(pstr) + 1);

        pstr = strtok_r(NULL, "", &psavestat2);
        if (pstr == NULL)
          break;
        strncpy(request->header[requestlenght].value, pstr, strlen(pstr) + 1);

        strtrim(request->header[requestlenght].value);

        requestlenght++;
      }
      request->headerlenght = requestlenght;
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

  if (start > 0 || end < (len - 1) && end > start) {
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';
  }
}
