#include "campi/webcam.h"

char*                   get_id() {

  FILE                  *file;
  char                  *id;

  file = fopen("/home/campie_camera/data/id", "r");
  id = calloc(36 + 1, sizeof(char));
  if (!file) {
    printf("Cannot open id file\n");
    memcpy(id, "NOID", 4);
    return (id);
  }
  fscanf(file, "%s", id);
  id[36] = '\0';
  fclose(file);
  return (id);
}
