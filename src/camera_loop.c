#include "campi/webcam.h"

void                    camera_loop(t_camera *camera, SOCKET sock) {

  time_t                last_time;
  int                   diff;
  uint8_t               *rgb;
  int                   first_loop;

  last_time = 0;
  first_loop = 1;
  while (1) {
    camera_frame(camera);
    if (first_loop) {
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
      first_loop = 0;
    }
    else if (time(NULL) - last_time >= 30 && camera->infos.state) {
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      diff = cmp_rgb(camera->prev, rgb, WIDTH, HEIGHT, CONTINUE_ON_LMT, camera->infos.precision);
      if ((diff * 100) /  (WIDTH * HEIGHT) >= camera->infos.precision) {
        printf("precision: %f\n", camera->infos.precision);
        printf("Move: %ld || %f\n", time(NULL), (diff * 100.f) /  (WIDTH * HEIGHT));
        save_current_jpeg(rgb, WIDTH, HEIGHT);
        send_image(time(NULL), sock, camera);
      }
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
      last_time = time(NULL);
    }
  }
}
