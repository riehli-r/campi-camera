#include "campi/webcam.h"

t_camera*                  open_device(const char *dev, uint32_t width, uint32_t height, char *label) {

  int                      fd;
  int                      i;
  t_camera                 *camera;

  fd = open(dev, O_RDWR | O_NONBLOCK, 0);
  if (fd == -1)
    exit_failure("fopen error");
  camera = malloc(sizeof(t_camera));
  if (!camera)
    exit_failure("camera malloc error");
  camera->infos.label = label;
  camera->fd = fd;
  camera->width = width;
  camera->height = height;
  camera->buffer_count = 0;
  camera->buffers = NULL;
  camera->head.length = 0;
  camera->head.start = NULL;
  camera->timeout.tv_sec = 1;
  camera->timeout.tv_usec = 0;

  init_device(camera);
  start_camera(camera);

  for (i = 0; i < INIT_FRAME; i++)
    camera_frame(camera);
  return (camera);
}
