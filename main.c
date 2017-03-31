#include "my_stream.h"

int                 main(int argc, char **argv) {

  t_camera          *camera;
  int               out;
  int               i;
  int               diff;
  char              *device;

  if (argc == 1)
    device = "/dev/video0";
  else
    device = argv[1];
  camera = open_device(device, WIDTH, HEIGHT);

  printf("Lancement de la procédure de détection:\n");

  for (i = 0; i < 100; i++) {
    camera_frame(camera);
    if (i > 1) {
      diff = cmp_rgb(camera->prev, camera->head.start, WIDTH, HEIGHT);
      if (diff >= PIXEL_DIFF) {
        printf("Mouvement !!!!\n");
        save_current_jpeg(camera->head.start, WIDTH, HEIGHT);
      }
    //  printf("Difference: %d\n", cmp_rgb(camera->prev, camera->head.start, 1920, 1080));
    }
    memcpy(camera->prev, camera->head.start, camera->head.length);
  }

  stop_camera(camera);
  return (0);
}
