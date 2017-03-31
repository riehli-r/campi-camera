#include "my_stream.h"

int                 main(int argc, char **argv) {

  t_camera          *camera;
  int               diff;
  char              *device;
  uint8_t           *rgb;
  time_t            last_time;

  if (argc == 1)
    device = "/dev/video0";
  else
    device = argv[1];
  camera = open_device(device, WIDTH, HEIGHT);

  printf("Detection:\n");
  last_time = 0;
  while(1) {
    camera_frame(camera);
    if (last_time != time(NULL)) {
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      diff = cmp_rgb(camera->prev, rgb, WIDTH, HEIGHT, CONTINUE_ON_LMT);
      if (diff >= PIXEL_DIFF) {
        printf("Move: %ld || %d\n", time(NULL), diff);
        save_current_jpeg(rgb, WIDTH, HEIGHT);
      }
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
      last_time = time(NULL);
    }
  }

  stop_camera(camera);
  return (0);
}
