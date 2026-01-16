#include <stdio.h>
#include <string.h>
#include <time.h>

void LOG_INFO_RAW(const char *message, const char *file, const int line) {
  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "%s [INFO] %s:%d %s\n", timeBuff, filename, line,
           message);

  printf("\e[0;32m%s\e[0m", buff);
}

void LOG_WARN_RAW(const char *message, const char *file, const int line) {
  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "%s [WARN] %s:%d %s\n", timeBuff, filename, line,
           message);

  printf("\e[0;33m%s\e[0m", buff);
}
void LOG_ERROR_RAW(const char *message, const char *file, const int line) {
  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "%s [ERROR] %s:%d %s\n", timeBuff, filename,
           line, message);

  printf("\e[0;31m%s\e[0m", buff);
}
void LOG_FATAL_RAW(const char *message, const char *file, const int line) {
  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "%s [FATAL] %s:%d %s\n", timeBuff, filename,
           line, message);

  printf("\e[1;91m%s\e[0m", buff);
}
