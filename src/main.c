#include "campi/webcam.h"

int                 main(int argc, char **argv) {

  t_camera          *camera;
  int               diff;
  char              *device;
  uint8_t           *rgb;
  time_t            last_time;
  int               i;

  get_infos(get_server_ip());

  if (argc < 2)
    return (-1);

  if (argc == 2)
    device = "/dev/video0";
  else
    device = argv[2];

  camera = open_device(device, WIDTH, HEIGHT, argv[1]);
  printf("Detection:\n");
  last_time = 0;
  for (i = 0; i < 20; i++) {
  //while(1) {
    camera_frame(camera);
    if (!i) {
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
    }
    if (last_time != time(NULL) && i) {
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      diff = cmp_rgb(camera->prev, rgb, WIDTH, HEIGHT, CONTINUE_ON_LMT);
      if ((diff * 100) /  (WIDTH * HEIGHT) >= 2.5) {
        printf("Move: %ld || %f\n", time(NULL), (diff * 100.f) /  (WIDTH * HEIGHT));
        save_current_jpeg(rgb, WIDTH, HEIGHT);
      }
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
      last_time = time(NULL);
    }
  }

  stop_camera(camera);
  return (0);
}
