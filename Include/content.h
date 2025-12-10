#ifndef CONTENT_H
#define CONTENT_H

#include "../Include/global.h"
#include <stddef.h>


unsigned char *getContent(char *path, enum statusCodes *statuscode,
                          size_t *outSize);

#endif // CONTENT_H
