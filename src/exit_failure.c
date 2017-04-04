#include "campi/webcam.h"

void                    exit_failure(const char *mesg) {

  fprintf(stderr, "%s : %d\n", mesg, errno);
  exit(EXIT_FAILURE);
}
