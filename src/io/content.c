#include "../Include/configuration.h"
#include "../Include/global.h"
#include "../Include/logging.h"
#include <linux/limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

bool shouldIgnore(const char *request_path);

unsigned char *getContent(const char *path, enum statusCodes *statuscode,
                          size_t *outSize) {
  // gets the path to the website source code (html, css etc.)
  char *APP = Config_getContentPath();
  char pathbuff[400];
  unsigned char *httpbody;
  *outSize = 0;
  snprintf(pathbuff, sizeof(pathbuff), "%s%s", APP, path);
  char *filepath = realpath(pathbuff, NULL);
  if (!filepath) {
    *statuscode = FILE_NOT_FOUND;
    return NULL;
  }

  int app_len = strlen(APP);
  if (strncmp(filepath, APP, app_len) != 0 ||
      ((filepath[app_len] != '/') && filepath[app_len] != '\0')) {
    *statuscode = FILE_NOT_FOUND;
    free(filepath);
    return NULL;
  }

  if (shouldIgnore(filepath)) {
    *statuscode = FILE_NOT_FOUND;
    free(filepath);
    return NULL;
  }

  FILE *file = fopen(filepath, "rb");
  free(filepath);

  if (file != NULL) {
    int fd = fileno(file);
    struct stat stat;
    fstat(fd, &stat);

    httpbody = malloc(stat.st_size);
    if (!httpbody) {
      LOG_FATAL("Unable to Malloc");
      *statuscode = INTERNAL_ERROR;
      fclose(file);
      return NULL;
    }

    int bytes = fread(httpbody, 1, stat.st_size, file);
    if (bytes <= 0) {
      *statuscode = FILE_NOT_FOUND;
      fclose(file);
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

bool shouldIgnore(const char *request_path) {
  request_path += strlen(Config_getRootFile());
  struct Ignores ignores = Config_getIgnores();

  for (int i = 0; i < Config_getIgnoreCount(); i++) {
    if (strstr(request_path, ignores.ignores[i])) {
      return true;
    }
  }

  return false;
}
