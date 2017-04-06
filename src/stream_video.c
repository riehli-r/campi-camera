#include "campi/webcam.h"

void                    stream_video(t_camera *camera) {

  SOCKET                sock;
  char                  *buffer;

  sock = *camera->sock;
  stop_camera(camera);
  camera = open_device("/dev/video0", 640, 480);
  camera->sock = &sock;
  camera->is_sending = 1;
  camera->infos.stream = 1;
  init_device(camera);
  start_camera(camera);
  buffer = calloc(BUFF_SIZE, sizeof(char));
  memcpy(buffer, "frame#start", 11);
  send(sock, buffer, 11, 0);
  reset_buffer(buffer);
  while (1) {
    printf("frame\n");
    camera_frame(camera);
    send_frame(sock, camera->head);
  }
  memcpy(buffer, "frame#end", 9);
  send(sock, buffer, 9, 0);
  stop_camera(camera);
}
