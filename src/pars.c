#include "../Include/http.h"
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 4096
enum status { METADATA, HEADER, BODY };

httpRequest *parshttp(const char *httprequest) {
  char buffer[1024];
  strcpy(buffer, httprequest);
  char *str;
  int status = METADATA;
  httpRequest *request = malloc(sizeof(httprequest));
  char *line;
  line = strtok(buffer, "\r\n");
  do {

    switch (status) {
    case METADATA: {

      str = strtok(line, " ");
      if (str != NULL) {
        strcpy(request->requestLine.method, str);
      }
      str = strtok(NULL, " ");
      if (str != NULL) {
        strcpy(request->requestLine.url, str);
      }
      str = strtok(NULL, " ");
      if (str != NULL) {
        strcpy(request->requestLine.version, str);
      }
      status = HEADER;
      break;
    }
    case HEADER:
      break;
    case BODY:
      break;
    }

  } while ((line = strtok(NULL, "\r\n")));
  return request;
}
