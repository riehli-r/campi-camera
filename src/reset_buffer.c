#include "campi/webcam.h"

void                    reset_buffer(char *buffer) {
  memset(buffer, '\0', BUFF_SIZE);
}
