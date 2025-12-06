#include "../Include/http.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFERSIZE 4096
enum status { METADATA, HEADER, BODY };

httpRequest *parshttp(char *httprequest) {
  char *pstr;
  int status = METADATA;
  httpRequest *request = malloc(sizeof(httpRequest));
  char *psavestat1 = httprequest;
  char *pline;
  char *psavestat2;

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
        strcpy(request->requestLine.method, pstr);
      } else {
        return NULL;
      }
      pstr = strtok_r(NULL, " ", &psavestat2);
      if (pstr != NULL) {
        strcpy(request->requestLine.url, pstr);
      } else {
        return NULL;
      }
      pstr = strtok_r(NULL, " ", &psavestat2);
      if (pstr != NULL) {
        strcpy(request->requestLine.version, pstr);
      } else {
        return NULL;
      }

      status = HEADER;
      break;
    }
    case HEADER:
      break;
    case BODY:
      break;
    }

  } while ((pline = strtok_r(NULL, "\r\n", &psavestat1)) != NULL);
  return request;
}
