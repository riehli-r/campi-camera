#include "my_stream.h"

int                     camera_capture(t_camera *camera) {

  struct v4l2_buffer    buf;

  memset(&buf, 0, sizeof buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (multi_ioctl(camera->fd, VIDIOC_DQBUF, &buf) == -1)
    return(0);
  memcpy(camera->head.start, camera->buffers[buf.index].start, buf.bytesused);
  camera->head.length = buf.bytesused;
  if (multi_ioctl(camera->fd, VIDIOC_QBUF, &buf) == -1)
    return (0);
  return(1);
}
