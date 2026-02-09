#ifndef HTTP_H
#define HTTP_H

#include "../Include/global.h"
#include <stddef.h>

typedef struct {
  char key[100];
  char value[100];
} Pair;

struct requestLine {
  char method[25];
  char path[150];
  char version[25];
};
typedef struct {
  struct requestLine requestLine;
  Pair header[25];
  int headerlenght;
  // body not implemented yeat
} httpRequest;

typedef struct {
  char *pResponse;
  unsigned char *pBody;
  int responseLenght;
} Response;

void fixNondirectpath(httpRequest *request);
char *getResponseFromError(enum statusCodes statuscodes, unsigned char **pbody,
                           size_t *bodySize);

// The MIMI types used for getting the content type
extern Pair G_MIME[];
extern const int G_MIME_COUNT;

#endif // !HTTP_H
