#include "../../Include/configuration.h"
#include "../../Include/global.h"
#include <string.h>
#include <unistd.h>

void readIncommingData(char *buff, const int clientfd, char *httprequest,
                       enum statusCodes *status) {
  int bytesread = 0;
  const int MAX_REQUEST_SIZE = Config_getMaxRequestSize();
  int n;

  if (MAX_REQUEST_SIZE - NULL_TERMINATOR <= 0) {
    *status = REQUEST_TO_BIG;
    return;
  }
  while ((n = read(clientfd, buff, MAX_REQUEST_SIZE - NULL_TERMINATOR)) > 0) {
    bytesread += n;

    if (bytesread > MAX_REQUEST_SIZE) {
      *status = REQUEST_TO_BIG;
      break;
    }

    buff[n] = '\0';
    strncat(httprequest, buff,
            MAX_REQUEST_SIZE - strlen(httprequest) - NULL_TERMINATOR);

    if (strstr(httprequest, "\r\n\r\n") != NULL) {
      break;
    }
  }
  *status = SUCCESS;
}
