#include "campi/webcam.h"

void                    camera_loop(t_camera *camera, SOCKET sock) {

  time_t                last_time;
  int                   diff;
  uint8_t               *rgb;
  int                   first_loop;
  uint32_t              width;
  uint32_t              height;
  char                  *filename;

  width = camera->width;
  height = camera->height;
  last_time = 0;
  first_loop = 1;
  while (1) {
    camera_frame(camera);
    if (first_loop) {
      rgb = yuyv_to_rgb(camera->head.start, width, height);
      memcpy(camera->prev, rgb, width * height * 3);
      first_loop = 0;
    }
    else if (time(NULL) - last_time >= 1 && (camera->infos.state || camera->take_picture)) {
      rgb = yuyv_to_rgb(camera->head.start, width, height);
      diff = cmp_rgb(camera->prev, rgb, width, height, CONTINUE_ON_LMT, camera->infos.precision);
      if ((diff * 100) /  (width * height) >= camera->infos.precision || camera->take_picture) {
        printf("precision: %f\n", camera->infos.precision);
        printf("Move: %ld || %f\n", time(NULL), (diff * 100.f) /  (width * height));
        filename = calloc(20, sizeof(char));
        sprintf(filename, "%ld", time(NULL));
        send_image(save_current_jpeg(rgb, width, height, filename), sock, camera);
        free(filename);
      }
      memcpy(camera->prev, rgb, width * height * 3);
      last_time = time(NULL);
    }
  }
}
