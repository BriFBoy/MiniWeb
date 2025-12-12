#include "../Include/httpbuilder.h"
#include "../Include/http.h"
#include <stdio.h>
#include <string.h>

void addStatusLine(char *response, const char *statusline,
                   int maxResponseLenght) {

  strncpy(response, statusline, maxResponseLenght);
  strncat(response, "\r\n", maxResponseLenght - strlen(response));
}

void addHeaderLine(char *response, const char *headerfield,
                   int maxResponseLenght) {

  strncat(response, headerfield, maxResponseLenght - strlen(response));
  strncat(response, "\r\n", maxResponseLenght - strlen(response));
}
void addContentType(char *response, const int maxResponseLenght,
                    const char *contentTypeValue) {
  char contentType[100];

  snprintf(contentType, sizeof(contentType), "Content-Type: %s\r\n",
           contentTypeValue);
  strncat(response, contentType, maxResponseLenght - strlen(response));
}
void addContentLenght(char *response, const int maxResponseLenght,
                      const int contentLenghtValue) {
  char contentLenght[100];

  snprintf(contentLenght, sizeof(contentLenght), "Content-Lenght: %d\r\n",
           contentLenghtValue);
  strncat(response, contentLenght, maxResponseLenght - strlen(response));
}

char *getDefaultHeaderFields() {
  return "Server: MiniWeb\r\n"
         "X-Content-Type-Options: nosniff\r\n"
         "Connection: close\r\n"
         "Cache-Control: private, max-age=60\r\n"
         "Referrer-Policy: no-referrer";
}
void createResponse(char *response, const int maxResponseLenght,
                    char *statusline, const char *path,
                    const int contentLenght) {

  addStatusLine(response, statusline, maxResponseLenght);
  addHeaderLine(response, getDefaultHeaderFields(), maxResponseLenght);
  addContentType(response, maxResponseLenght, getContentType(path));
  addContentLenght(response, maxResponseLenght, contentLenght);
  strncat(response, "\r\n", maxResponseLenght - strlen(response));
}

const char *getContentType(const char *path) {
  const char *fileextention = strrchr(path, '.');
  if (fileextention) {
    for (int i = 0; i < (sizeof(G_MINE) / sizeof(G_MINE[0])); i++) {

      if (strcmp(G_MINE[i].key, fileextention) == 0) {
        printf("%s\n", G_MINE[i].value);
        return G_MINE[i].value;
      }
    }
  }
  return "application/octet-stream";
}
