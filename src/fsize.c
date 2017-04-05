#include "campi/webcam.h"

int                     fsize(FILE *file) {

  int                   nbr;
  fseek(file, 0, SEEK_END);
  nbr = ftell(file);
  fseek(file, 0, SEEK_SET);
  return (nbr);
}
