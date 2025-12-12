
#include "../Include/global.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// need to fix for binary files/images
unsigned char *getContent(char *path, enum statusCodes *statuscode,
                          size_t *outSize) {
  // gets the path of all the html, css, e.t.c
  const char *APP = getenv("MINIWEB_SOURCE");
  char filepath[1024];
  unsigned char *httpbody;

  snprintf(filepath, sizeof(filepath), "%s%s", APP, path);
  FILE *file = fopen(filepath, "rb");
  if (file != NULL) {
    int fd = fileno(file);
    struct stat stat;
    fstat(fd, &stat);
    *outSize = stat.st_size;

    httpbody = malloc(stat.st_size);
    if (httpbody) {
      fread(httpbody, 1, stat.st_size, file);
    } else {
      printf("Failed to malloc\n");
      *statuscode = INTERNAL_ERROR;
      return NULL;
    }

    fclose(file);
    return httpbody;
  } else {
    printf("Failed to open file\n");
    *statuscode = FILE_NOT_FOUND;
    return NULL;
  }
}
