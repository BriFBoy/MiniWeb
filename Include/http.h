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

// Response struct
typedef struct {
  char *pResponse;
  unsigned char *pBody;
  int responseLenght;
} Response;

void fixNondirectpath(httpRequest *request);
char *getResponseFromError(enum statusCodes statuscodes, unsigned char **pbody,
                           size_t *bodySize);

static Pair G_MIME[] = {{".html", "text/html"},
                        {".css", "text/css"},
                        {".ico", "image/x-icon"},
                        {".png", "image/png"},
                        {".jpg", "image/jpg"},
                        {".jpeg", "image/jpeg"}};


#endif // !HTTP_H
