#ifndef HTTP_H
#define HTTP_H

#include "../Include/global.h"

typedef struct {
  char key[200];
  char value[200];
} Pair;

struct requestLine {
    char method[25];
    char path[200];
    char version[25];
};
typedef struct {
  struct requestLine requestLine;
  Pair header[50];
  int headerlenght;
  // body not implemented yeat
} httpRequest;

// Response struct
typedef struct {
  char *pResponse;
  char *pBody;
  int responseLenght;
} Response;

void fixNondirectpath(httpRequest *request);
char *getResponseFromError(enum statusCodes statuscodes, int *responselenght);

#endif // !HTTP_H
