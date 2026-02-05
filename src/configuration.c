#include "../Include/configuration.h"
#include "../Include/logging.h"
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static Config cfg;
static bool config_Loaded = false;

void load_config(const char *path) {
  if (config_Loaded) {
    return;
  }
  FILE *file = fopen(path, "r");
  char buff[1024];
  Config tmp_Config = {0};

  if (!file) {
    LOG_WARN("NO CONFIG FILE FOUND");
    return;
  }
  fread(buff, sizeof(buff), 1, file);

  const cJSON *parsed = cJSON_Parse(buff);
  cJSON *json_loglevel = cJSON_GetObjectItemCaseSensitive(parsed, "loglevel");

  if (cJSON_IsString(json_loglevel) && json_loglevel->string != NULL) {
    if (strcmp(json_loglevel->valuestring, "info") == 0) {
      tmp_Config.loglevel = INFO;
    } else if (strcmp(json_loglevel->valuestring, "warn") == 0) {
      tmp_Config.loglevel = WARN;
    } else if (strcmp(json_loglevel->valuestring, "error") == 0) {
      tmp_Config.loglevel = ERROR;
    } else if (strcmp(json_loglevel->valuestring, "fatal") == 0) {
      tmp_Config.loglevel = FATAL;
    } else {
      LOG_ERROR("Incorrect value in config.json: loglevel");
      tmp_Config.loglevel = INFO;
    }
  }

  cfg = tmp_Config;
  config_Loaded = true;
}

int Config_getLogLevel() { return cfg.loglevel; }
