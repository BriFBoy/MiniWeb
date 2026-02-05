#ifndef __CONFIGURATION__
#define __CONFIGURATION__

void load_config(const char *path);
int Config_getLogLevel();

enum loglevel { INFO, WARN, ERROR, FATAL };
typedef struct {
  enum loglevel loglevel;
} Config;

#endif // !__CONFIGURATION__
