#include "campi/webcam.h"

t_camera*                  open_device(const char *dev, uint32_t width, uint32_t height, SOCKET sock) {

  int                      fd;
  t_camera                 *camera;

  fd = open(dev, O_RDWR | O_NONBLOCK, 0);
  if (fd == -1)
    exit_failure("ffopen error");
  camera = malloc(sizeof(t_camera));
  if (!camera)
    exit_failure("camera malloc error");
  camera->infos.id = get_id();
  camera->infos.label = calloc(100, sizeof(char));
  camera->infos.state = 1;
  camera->infos.precision = 2.5;
  camera->infos.stream = 0;
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
  camera->take_picture = 0;
  pthread_mutex_init(&camera->mutex, NULL);
  pthread_cond_init(&camera->cond, NULL);

  camera->sock = &sock;
  camera->cl = client(&sock, 1);
  camera->cl.param = camera;

  add_callback(&camera->cl, "id", &set_id);
  add_callback(&camera->cl, "set-label", &set_label);
  add_callback(&camera->cl, "set-state", &set_state);
  add_callback(&camera->cl, "set-precision", &set_precision);

  set_label(camera, "label#DEFAULT_LABEL");
  return (camera);
}
