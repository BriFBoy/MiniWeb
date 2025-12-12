#include "../Include/httpbuilder.h"
#include "../Include/http.h"
#include <stdio.h>
#include <string.h>

void addStatusLine(char *response, const char *statusline,
                   int maxResponseLenght) {

  strncpy(response, statusline, maxResponseLenght);
  response[strlen(statusline)] = '\0';
  strncat(response, "\r\n", maxResponseLenght - strlen(response));
}

void addHeaderLine(char *response, const char *headerfield,
                   int maxResponseLenght) {

  strncat(response, headerfield, maxResponseLenght - strlen(response));
  strncat(response, "\r\n", maxResponseLenght - strlen(response));
}
void addContentTypeAndLenght(char *response, const int maxResponseLenght,
                             const char *contentTypeValue,
                             const int contentLenghtValue) {
  char contentType[100];
  char contentLenght[100];

  snprintf(contentType, sizeof(contentType), "Content-Type: %s\r\n",
           contentTypeValue);
  strncat(response, contentType, maxResponseLenght - strlen(response));

  snprintf(contentLenght, sizeof(contentLenght), "Content-Length: %d\r\n",
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
void createResponseHeader(char *response, const int maxResponseLenght,
                          char *statusline, const char *path,
                          const int contentLenght) {

  addStatusLine(response, statusline, maxResponseLenght);
  addHeaderLine(response, getDefaultHeaderFields(), maxResponseLenght);
  addContentTypeAndLenght(response, maxResponseLenght, getContentType(path),
                          contentLenght);
  strncat(response, "\r\n", maxResponseLenght - strlen(response));
}

const char *getContentType(const char *path) {
  const char *fileextention = strrchr(path, '.');
  if (fileextention) {
    for (int i = 0; i < (sizeof(G_MIME) / sizeof(G_MIME[0])); i++) {

      if (strcmp(G_MIME[i].key, fileextention) == 0) {
        printf("%s\n", G_MIME[i].value);
        return G_MIME[i].value;
      }
    }
  }
  return "application/octet-stream";
}
