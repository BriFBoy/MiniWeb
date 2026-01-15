#include <stdio.h>
#include <time.h>

void LOG_INFO_RAW(const char *message, const char *file, const int line) {
  char buff[200];
  char timeBuff[25];

  time_t tSeconds = time(NULL);
  struct tm *t = localtime(&tSeconds);
  strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", t);

  snprintf(buff, sizeof(buff), "%s [INFO] %s:%d %s\n", timeBuff, file, line, message);

  printf("\e[0;32m%s\e[0m", buff);
}
