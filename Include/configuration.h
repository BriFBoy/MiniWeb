#ifndef __CONFIGURATION__
#define __CONFIGURATION__

void load_config(const char *path);
int Config_getLogLevel();
int Config_getPort();
int Config_getTimeout();
int Config_getMaxRequestSize();
char *Config_getContentPath();
char *Config_getRootFile();

enum loglevel { INFO, WARN, ERROR, FATAL };
typedef struct {
  enum loglevel loglevel;
  int port;
  int timeout;
  int max_request_size;
  char content_path[100];
  char root_file[25];
} Config;

#endif // !__CONFIGURATION__
