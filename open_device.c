#include "my_stream.h"

t_camera*                     open_device(const char *dev, uint32_t width, uint32_t height) {

  int                fd;
  t_camera           *camera;

  fd = open(dev, O_RDWR | O_NONBLOCK, 0);
  if (fd == -1)
    exit_failure("fopen error");
  camera = malloc(sizeof(t_camera));
  if (!camera)
    exit_failure("camera malloc error");
  camera->fd = fd;
  camera->width = width;
  camera->height = height;
  camera->buffer_count = 0;
  camera->buffers = NULL;
  camera->head.length = 0;
  camera->head.start = NULL;

  return (camera);
}
