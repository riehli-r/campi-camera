#include "campi/webcam.h"

void                    start_camera(t_camera *camera) {

  size_t                i;
  struct v4l2_buffer    buf;
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  for (i = 0; i < camera->buffer_count; i++) {
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (multi_ioctl(camera->fd, VIDIOC_QBUF, &buf) == -1)
      exit_failure("VIDIOC_QBUF");
  }

  if (multi_ioctl(camera->fd, VIDIOC_STREAMON, &type) == -1)
    exit_failure("VIDIOC_STREAMON");
}
