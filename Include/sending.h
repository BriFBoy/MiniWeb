#ifndef SENDING_H
#define SENDING_H

#include "http.h"

void send_error_message(const int clientfd, const enum statusCodes error);
void sendResponse(const int clientfd, const httpRequest *request);

#endif // !SENDING_H
