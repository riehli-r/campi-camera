#include "campi/webcam.h"

char*                   get_id() {

  FILE                  *file;
  char                  *id;

  file = fopen("data/id", "r");
  id = calloc(36 + 1, sizeof(char));
  if (file == NULL) {
    printf("Cannot open file\n");
    return (id);
  }
  fscanf(file, "%s", id);
  id[36] = '\0';
  fclose(file);
  return (id);
}
