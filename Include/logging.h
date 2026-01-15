#ifndef LOGGING_H
#define LOGGING_H

#define LOG_INFO(msg, ...) LOG_INFO_RAW(msg, __FILE__, __LINE__ ##__VA_ARGS__);

void LOG_INFO_RAW(const char *message, const char *file, const int line);

#endif // !LOGGING_H
