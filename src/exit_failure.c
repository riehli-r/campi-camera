#include "campi/webcam.h"

void                    exit_failure(const char *mesg) {

  fprintf(stderr, "%s\n", mesg);
  exit(EXIT_FAILURE);
}
