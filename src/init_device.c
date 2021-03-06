#include "campi/webcam.h"


void                     capability_requests(t_camera *camera) {

  struct v4l2_capability cap;

  if (multi_ioctl(camera->fd, VIDIOC_QUERYCAP, &cap) == -1)
    exit_failure("VIDIOC_QUERYCAP");
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    exit_failure("V4L2_CAP_VIDEO_CAPTURE");
  if (!(cap.capabilities & V4L2_CAP_STREAMING))
    exit_failure("V4L2_CAP_STREAMING");
  printf("Camera name: %s\n", (char*)cap.card);
}

void                     formarq_request(t_camera *camera) {

  struct v4l2_format     format;

  memset(&format, 0, sizeof format);
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (!camera->infos.stream) {
    format.fmt.pix.width = camera->width;
    format.fmt.pix.height = camera->height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  }
  else {

    printf("Stream\n");
    format.fmt.pix.width = 640;
    format.fmt.pix.height = 480;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  }
  format.fmt.pix.field = V4L2_FIELD_NONE;
  if (multi_ioctl(camera->fd, VIDIOC_S_FMT, &format) == -1)
    exit_failure("VIDIOC_S_FMT");
}

void                     buffer_request(t_camera *camera) {

  struct v4l2_requestbuffers req;

  memset(&req, 0, sizeof req);
  req.count = 4;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  if (multi_ioctl(camera->fd, VIDIOC_REQBUFS, &req) == -1)
    exit_failure("VIDIOC_REQBUFS");
  camera->buffer_count = req.count;
  camera->buffers = calloc(req.count, sizeof(t_buffer));
}

void                     init_device(t_camera *camera) {

  size_t                 buf_max = 0;
  size_t                 i;

  capability_requests(camera);
  formarq_request(camera);
  buffer_request(camera);

  buf_max = 0;
  for (i = 0; i < camera->buffer_count; i++) {
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (multi_ioctl(camera->fd, VIDIOC_QUERYBUF, &buf) == -1)
      exit_failure("VIDIOC_QUERYBUF");
    if (buf.length > buf_max)
      buf_max = buf.length;
    camera->buffers[i].length = buf.length;
    camera->buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camera->fd, buf.m.offset);
    if (camera->buffers[i].start == MAP_FAILED)
      exit_failure("mmap error");
  }
  camera->head.start = malloc(buf_max);
  camera->prev = malloc(camera->width * camera->height * 3);
}
