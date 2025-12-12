#ifndef HTTPBUILDER_H
#define HTTPBUILDER_H

#include <stddef.h>

void addStatusLine(char *response, const char *statusline, int maxResponseLenght);
void addHeaderLine(char *response, const char *headerfield, int maxResponseLenght);
void addContentType(char *response, const int maxResponseLenght,
                    const char *contentType);
void addBody(char *response, size_t responseSize, const unsigned char *body,
             size_t bodySize, int maxResponseLenght);
char *getDefaultHeaderFields(); 
const char *getContentType(const char *path);
void addContentLenght(char *response, const int maxResponseLenght,
                      const int contentLenght);
void createResponse(char *response, const int maxResponseLenght,
                    char *statusline, const char *path,
                    const int contentLenght);

#endif // !HTTPBUILDER_H

