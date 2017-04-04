#include "campi/webcam.h"

void                    save_id(char *id) {

  FILE                  *file;

  file = fopen("data/id", "w");
  if (file)
    fprintf(file, "%s", id);
  fclose(file);
}
