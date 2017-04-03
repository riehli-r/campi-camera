#include "campi/webcam.h"

char*                     get_id() {

  FILE                    *file;
  char                    *id;

  file = fopen("data/id", "r");
  if (file == NULL) {
    printf("Cannot open file\n");
    return (NULL);
  }
  id = calloc(36 + 1, sizeof(char));
  fscanf(file, "%s", id);
  id[36] = '\0';
  fclose(file);
  return (id);
}


t_camera*                  open_device(const char *dev, uint32_t width, uint32_t height) {

  int                      fd;
  t_camera                 *camera;

  fd = open(dev, O_RDWR | O_NONBLOCK, 0);
  if (fd == -1)
    exit_failure("fopen error");
  camera = malloc(sizeof(t_camera));
  if (!camera)
    exit_failure("camera malloc error");
  camera->infos.id = get_id();
//  printf("%s\n", camera->infos.id);
  camera->infos.label = DEFAULT_LABEL;
  camera->fd = fd;
  camera->width = width;
  camera->height = height;
  camera->buffer_count = 0;
  camera->buffers = NULL;
  camera->head.length = 0;
  camera->head.start = NULL;
  camera->timeout.tv_sec = 1;
  camera->timeout.tv_usec = 0;
  return (camera);
}
