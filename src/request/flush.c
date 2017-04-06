#include "request.h"

void              flush(char *buff) {
  memset(buff, '\0', BUFF_SIZE);
}
