
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *getContent(char *path) {
  // gets the path of all the html, css, e.t.c
  const char *APP = getenv("MINIWEB_SOURCE");
  char filepath[200];
  char filebuff[1024];
  char *httpbody = malloc(sizeof(char));
  char *tmp;
  httpbody[0] = '\0';

  snprintf(filepath, sizeof(filepath), "%s%s", APP, path);
  FILE *file = fopen(filepath, "r");
  if (file != NULL) {
    while (fgets(filebuff, sizeof(filebuff), file) != NULL) {
      tmp = realloc(httpbody, strlen(filebuff) + 1 + strlen(httpbody));
      if (!tmp) {
        printf("Failed to realloc in getContent\n");
        exit(1);
      }
      httpbody = tmp;
      strcat(httpbody, filebuff);
    }

    fclose(file);
    return httpbody;
  } else {
    printf("Failed to open file\n");
    free(httpbody);
  }
  return NULL;
}
