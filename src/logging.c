#include "../Include/configuration.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void LOG_INFO_RAW(const char *message, const char *file, const int line) {
  if (Config_getLogLevel() > INFO) {
    return;
  }

  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%F %H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "[%s] [\e[0;32mINFO\e[0m] %s:%d %s\n", timeBuff,
           filename, line, message);

  printf("%s", buff);
}

void LOG_WARN_RAW(const char *message, const char *file, const int line) {
  if (Config_getLogLevel() > WARN) {
    return;
  }
  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%F %H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "[%s] [\e[0;33mWARN\e[0m] %s:%d %s\n", timeBuff,
           filename, line, message);

  printf("%s", buff);
}
void LOG_ERROR_RAW(const char *message, const char *file, const int line) {
  if (Config_getLogLevel() >= ERROR) {
    return;
  }
  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%F %H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "[%s] [\e[0;31mERROR\e[0m] %s:%d %s\n", timeBuff,
           filename, line, message);

  printf("%s", buff);
}
void LOG_FATAL_RAW(const char *message, const char *file, const int line) {
  if (Config_getLogLevel() > FATAL) {
    return;
  }
  char buff[200];
  char timeBuff[25];
  char *filename;
  char filebuff[50];
  char *filebuff2;

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%F %H:%M:%S", t);

  strncpy(filebuff, file, sizeof(filebuff));
  filebuff[sizeof(filebuff) - 1] = '\0';
  if ((filename = strstr(filebuff, "/")) != NULL) {
    filename = filename + 1;
  }

  if ((filebuff2 = strstr(filebuff, ".")) != NULL) {
    *filebuff2 = '\0';
  }
  snprintf(buff, sizeof(buff), "[%s] [\e[1;91mFATAL\e[0m] %s:%d %s\n", timeBuff,
           filename, line, message);

  printf("%s", buff);
}
