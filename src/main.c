#include "campi/webcam.h"

int                 main(int argc, char **argv) {

  t_camera          *camera;
  char              *device;

  if (argc == 1)
    device = "/dev/video0";
  else
    device = argv[1];

  camera = open_device(device, WIDTH, HEIGHT);
  get_infos(get_server_ip(), camera);
  init_device(camera);
  start_camera(camera);
  //camera_loop(camera);

  stop_camera(camera);
  return (0);
}
