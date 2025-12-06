#ifndef HTTP_H
#define HTTP_H
typedef struct {
  char key[200];
  char value[200];
} Pair;

struct requestLine {
    char method[25];
    char url[200];
    char version[25];
};
typedef struct {
  struct requestLine requestLine;
  Pair header[50];
  int headerlenght;
  // body not implemented yeat
} httpRequest;

#endif // !HTTP_H
