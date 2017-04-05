#include "campi/webcam.h"

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
  camera->infos.label = calloc(100, sizeof(char));
  camera->infos.state = 1;
  camera->infos.precision = PIXEL_DIFF;
  camera->fd = fd;
  camera->width = width;
  camera->height = height;
  camera->buffer_count = 0;
  camera->buffers = NULL;
  camera->head.length = 0;
  camera->head.start = NULL;
  camera->timeout.tv_sec = 1;
  camera->timeout.tv_usec = 0;
  camera->is_sending = 0;

  pthread_mutex_init(&camera->mutex, NULL);
  pthread_cond_init(&camera->cond, NULL);

  set_label(camera, "label#DEFAULT_LABEL");
  return (camera);
}
