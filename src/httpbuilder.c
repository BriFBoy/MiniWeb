#include "../Include/http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addStatusLine(char *response, const char *statusline,
                   int maxResponseLenght) {

  strncpy(response, statusline, maxResponseLenght);
  strncat(response, "\r\n", maxResponseLenght);
}

void addHeaderLine(char *response, const char *headerfield,
                   int maxResponseLenght) {

  strncat(response, headerfield, maxResponseLenght);
  strncat(response, "\r\n", maxResponseLenght);
}
void addContentType(char *response, const int maxResponseLenght,
                    const char *contentType) {
  char Content_Type[100];

  snprintf(Content_Type, sizeof(Content_Type), "Content-Type: %s\r\n",
           contentType);
  strncat(response, Content_Type, maxResponseLenght);
}
void addContentLenght(char *response, const int maxResponseLenght,
                      const int contentLenght) {
  char Content_Type[100];

  snprintf(Content_Type, sizeof(Content_Type), "Content-Lenght: %d\r\n",
           contentLenght);
  strncat(response, Content_Type, maxResponseLenght);
}

void addBody(char *response, const char *body, int maxResponseLenght) {

  strncat(response, "\r\n", maxResponseLenght);
  strncat(response, body, maxResponseLenght);
}

char *getDefaultHeaderFields() {
  return "Server: MyServer/1.0\r\n"
         "X-Content-Type-Options: nosniff\r\n"
         "Referrer-Policy: no-referrer";
}

const char *getContentType(const char *path) {
  const char *fileextention = strrchr(path, '.');
  if (fileextention) {

    for (int i = 0; i < (sizeof(MINE) / sizeof(MINE[0])); i++) {
      if (strcmp(MINE[i].key, fileextention) == 0) {
        return MINE[i].value;
      }
    }
  }
  return "application/octet-stream";
}
