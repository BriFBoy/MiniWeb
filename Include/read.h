#ifndef READ_H
#define READ_H
#include "./global.h"

void readIncommingData(char *buff, const int clientfd, char *httprequest,
                       enum statusCodes *status);

#endif // !READ_H
