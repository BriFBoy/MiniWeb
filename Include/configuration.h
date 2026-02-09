#ifndef __CONFIGURATION__
#define __CONFIGURATION__

void load_config(const char *path);
int Config_getLogLevel();
int Config_getPort();
int Config_getTimeout();
int Config_getMaxRequestSize();
char *Config_getContentPath();
char *Config_getRootFile();
int Config_getthread_amount();
char *Config_getError(int error);
int Config_getIgnoreCount();
struct Ignores Config_getIgnores();

enum loglevel { INFO, WARN, ERROR, FATAL };
typedef struct {
  int code;
  char path[100];
} Error_Path;

struct Ignores {
  char ignores[10][100];
};

typedef struct {
  enum loglevel loglevel;
  int port;
  int timeout;
  int max_request_size;
  char content_path[100];
  char root_file[25];
  int thread_amount;
  Error_Path errors[10];
  struct Ignores ignores;
  int ignore_cont;
} Config;

#endif // !__CONFIGURATION__
