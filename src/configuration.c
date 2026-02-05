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

  cJSON *parsed = cJSON_Parse(buff);
  cJSON *json_loglevel = cJSON_GetObjectItemCaseSensitive(parsed, "loglevel");
  cJSON *json_port = cJSON_GetObjectItemCaseSensitive(parsed, "port");
  cJSON *json_timeout = cJSON_GetObjectItemCaseSensitive(parsed, "timeout");
  cJSON *json_max_request_size =
      cJSON_GetObjectItemCaseSensitive(parsed, "max_request_size");

  if (cJSON_IsNumber(json_timeout)) {
    tmp_Config.timeout = json_timeout->valueint;
  } else {
    LOG_ERROR("Incorrect value in config.json: timeout");
    tmp_Config.timeout = 5;
  }

  if (cJSON_IsNumber(json_max_request_size)) {
    tmp_Config.max_request_size = json_max_request_size->valueint;
  } else {
    LOG_ERROR("Incorrect value in config.json: max_request_size");
    tmp_Config.max_request_size = 1048576;
  }

  if (cJSON_IsNumber(json_port)) {
    tmp_Config.port = json_port->valueint;
  } else {
    LOG_ERROR("Incorrect value in config.json: port");
    tmp_Config.port = 8080;
  }

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
  } else {
    tmp_Config.loglevel = INFO;
  }

  cfg = tmp_Config;
  config_Loaded = true;
  cJSON_Delete(parsed);
}

int Config_getLogLevel() { return cfg.loglevel; }
int Config_getPort() { return cfg.port; }
int Config_getTimeout() { return cfg.timeout; }
int Config_getMaxRequestSize() { return cfg.max_request_size; }
