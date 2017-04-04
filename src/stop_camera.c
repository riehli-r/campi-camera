#include "campi/webcam.h"

void                    stop_camera(t_camera *camera) {

  enum v4l2_buf_type    type;
  size_t                i;

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (multi_ioctl(camera->fd, VIDIOC_STREAMOFF, &type) == -1)
    exit_failure("VIDIOC_STREAMOFF");

  for (i = 0; i < camera->buffer_count; i++)
    munmap(camera->buffers[i].start, camera->buffers[i].length);
  free(camera->infos.id);
  free(camera->infos.label);
  free(camera->buffers);
  camera->buffer_count = 0;
  camera->buffers = NULL;
  free(camera->head.start);
  camera->head.length = 0;
  camera->head.start = NULL;
  free(camera->prev);

  if (close(camera->fd) == -1)
    exit_failure("Close file");
  free(camera);
}
