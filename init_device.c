#include "my_stream.h"


void                     capability_requests(t_camera *camera) {

  struct v4l2_capability cap;

  if (multi_ioctl(camera->fd, VIDIOC_QUERYCAP, &cap) == -1)
    exit_failure("VIDIOC_QUERYCAP");
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    exit_failure("V4L2_CAP_VIDEO_CAPTURE");
  if (!(cap.capabilities & V4L2_CAP_STREAMING))
    exit_failure("V4L2_CAP_STREAMING");
  printf("Driver: %s\n", (char*)cap.driver);
  printf("Camera name: %s\n", (char*)cap.card);
}

void                     cropcap_requests(t_camera *camera) {
  struct v4l2_cropcap    cropcap;
  struct v4l2_crop       crop;

  memset(&cropcap, 0, sizeof(cropcap));
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (!multi_ioctl(camera->fd, VIDIOC_CROPCAP, &cropcap)) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect;
    if (multi_ioctl(camera->fd, VIDIOC_S_CROP, &crop) == -1)
      printf("Cropping not supported\n");
    else
      printf("Cropping supported\n");
  }
}

void                     format_request(t_camera *camera) {

  struct v4l2_format     format;

  memset(&format, 0, sizeof format);
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width = camera->width;
  format.fmt.pix.height = camera->height;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
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
  cropcap_requests(camera);
  format_request(camera);
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
}
