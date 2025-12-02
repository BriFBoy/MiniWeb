
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *getContent(char *path) {
  const char *HOME = getenv("HOME");
  const char *APP = getenv("APP");
  char filepath[200];
  char filebuff[1024];
  char *httpbody = malloc(sizeof(char));
  char *tmp;
  httpbody[0] = '\0';

  snprintf(filepath, sizeof(filepath), "%s%s%s", HOME, APP, path);
  FILE *file = fopen(filepath, "r");
  if (file != NULL) {
    while (fgets(filebuff, sizeof(filebuff), file) != NULL) {
      tmp = realloc(httpbody, strlen(filebuff) + 1 + strlen(httpbody));
      if (!tmp) {
        printf("Failed to realloc in getContent");
        exit(1);
      }
      httpbody = tmp;
      strcat(httpbody, filebuff);
    }

    fclose(file);
    return httpbody;
  } else {
    printf("Failed to open file");
    free(httpbody);
  }
  return NULL;
}
