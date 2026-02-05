#ifndef __CONFIGURATION__
#define __CONFIGURATION__

void load_config(const char *path);
int Config_getLogLevel();
int Config_getPort();
int Config_getTimeout();
int Config_getMaxRequestSize();

enum loglevel { INFO, WARN, ERROR, FATAL };
typedef struct {
  enum loglevel loglevel;
  int port;
  int timeout;
  int max_request_size;
} Config;

#endif // !__CONFIGURATION__
