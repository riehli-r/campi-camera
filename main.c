#include "my_stream.h"

int                 main() {

  t_camera          *camera;
  struct timeval    timeout;
  uint8_t           *rgb;
  FILE              *out;

  camera = open_device("/dev/video1", 1920, 1080);
  init_device(camera);
  start_camera(camera);

  /* skip 5 frames for booting a cam */
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  for (int i = 0; i < 5; i++) {
    camera_frame(camera, timeout);
  }
  camera_frame(camera, timeout);

  rgb = yuyv_to_rgb(camera->head.start, camera->width, camera->height);
  out = fopen("result.jpg", "w");
  write_jpeg_file(out, rgb, camera);
  fclose(out);
  free(rgb);

  stop_camera(camera);
  return (0);
}
