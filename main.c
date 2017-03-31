#include "my_stream.h"


void                display_jpg(t_buffer jpg) {
  size_t            i;

  for (i = 0; i < jpg.length; i++) {
    printf("%02x ", jpg.start[i]);
  }
  printf("/n");
}

void                display_rgb(uint8_t *rgb, uint32_t width, uint32_t height) {
  size_t            i;

  for (i = 0; i < width * height * 3; i++) {
    printf("%02x ", rgb[i]);
  }
  printf("/n");
}

int                 main() {

  t_camera          *camera;
  struct timeval    timeout;
  int               out;
  int               i;
  uint8_t           *rgb1;
  uint8_t           *rgb2;
  int               diff;

  camera = open_device("/dev/video0", 1920, 1080);
  init_device(camera);
  start_camera(camera);

  /* skip 5 frames for booting a cam */
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  for (i = 0; i < 5; i++)
    camera_frame(camera, timeout);

  for (i = 0; i < 100; i++) {
    camera_frame(camera, timeout);
    usleep(2000);
    if (i > 1) {
      diff = cmp_rgb(camera->prev, camera->head.start, 1920, 1080);
      if (diff > 100000) {
        printf("Mouvement !!!!\n");
      }
    //  printf("Difference: %d\n", cmp_rgb(camera->prev, camera->head.start, 1920, 1080));
    }
    memcpy(camera->prev, camera->head.start, camera->head.length  );
  }

/*
  camera_frame(camera, timeout);
  rgb = yuyv_to_rgb(camera->head.start, camera->width, camera->height);
  out = open("result.jpg", O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  write_jpeg_file(out, camera);
  display_rgb(rgb, 640, 480);
*/
  stop_camera(camera);
  return (0);
}
