#include "../Include/configuration.h"
#include "../Include/global.h"
#include "../Include/logging.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned char *getContent(const char *path, enum statusCodes *statuscode,
                          size_t *outSize) {
  // gets the path to the website source code (html, css etc.)
  const char *APP = Config_getContentPath();
  char filepath[1024];
  unsigned char *httpbody;
  *outSize = 0;

  snprintf(filepath, sizeof(filepath), "%s%s", APP, path);

  // checks to see if the file is inside the website root directory
  if (strstr(filepath, "..") != NULL) {
    *statuscode = FILE_NOT_FOUND;
    return NULL;
  }
  if (strncmp(filepath, APP, strlen(APP)) != 0) {
    *statuscode = FILE_NOT_FOUND;
    return NULL;
  }

  FILE *file = fopen(filepath, "rb");
  if (file != NULL) {
    int fd = fileno(file);
    struct stat stat;
    fstat(fd, &stat);

    httpbody = malloc(stat.st_size);
    if (!httpbody) {
      LOG_FATAL("Unable to Malloc");
      *statuscode = INTERNAL_ERROR;
      return NULL;
    }

    int bytes = fread(httpbody, 1, stat.st_size, file);
    if (bytes <= 0) {
      *statuscode = FILE_NOT_FOUND;
      return NULL;
    }

    *outSize = stat.st_size;
    fclose(file);
    return httpbody;
  } else {
    LOG_WARN("Failed to get file");
    *statuscode = FILE_NOT_FOUND;
    return NULL;
  }
}
