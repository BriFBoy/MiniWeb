#ifndef LOGGING_H
#define LOGGING_H

#define LOG_INFO(msg, ...) LOG_INFO_RAW(msg, __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_WARN(msg, ...) LOG_WARN_RAW(msg, __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_ERROR(msg, ...) LOG_ERROR_RAW(msg, __FILE__, __LINE__, ##__VA_ARGS__);
#define LOG_FATAL(msg, ...) LOG_FATAL_RAW(msg, __FILE__, __LINE__, ##__VA_ARGS__);

void LOG_INFO_RAW(const char *message, const char *file, const int line);
void LOG_WARN_RAW(const char *message, const char *file, const int line);
void LOG_ERROR_RAW(const char *message, const char *file, const int line);
void LOG_FATAL_RAW(const char *message, const char *file, const int line);

#endif // !LOGGING_H
