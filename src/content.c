
#include "../Include/global.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

// need to fix for binary files/images
char *getContent(char *path, enum statusCodes *statuscode, int *outSize) {
  // gets the path of all the html, css, e.t.c
  const char *APP = getenv("MINIWEB_SOURCE");
  char filepath[1024];
  char *httpbody;

  snprintf(filepath, sizeof(filepath), "%s%s", APP, path);
  FILE *file = fopen(filepath, "rb");
  if (file != NULL) {
    int fd = fileno(file);
    struct stat stat;
    fstat(fd, &stat);
    *outSize = stat.st_size;
    printf("file size: %ld\n", stat.st_size);

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
