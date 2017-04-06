#include "campi/webcam.h"

void                    send_frame(SOCKET sock, t_buffer head) {
  char                  *buffer;
  size_t                i;
  char                  *base64;

  base64 = base64_encode((char*)head.start, head.length);
  buffer = calloc(BUFF_SIZE + 6, sizeof(char));
  printf("FRAME SIZE: %ld\n", strlen(base64));
  for (i = 0; i < strlen(base64); i += BUFF_SIZE) {
    memset(buffer, '\0', BUFF_SIZE + 6);
    memcpy(buffer, "frame#", 6);
    strncat(buffer, &base64[i], BUFF_SIZE);
    send(sock, buffer, BUFF_SIZE + 6, 0);
    memset(buffer, '\0', BUFF_SIZE + 6);
    usleep(10500);
    //recv(sock, buffer, BUFF_SIZE + 6, 0);
  }
  memset(buffer, '\0', BUFF_SIZE + 6);
  memcpy(buffer, "frame#end", 9);
  send(sock, buffer, 9, 0);
  free(base64);
  free(buffer);
}

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


int                     main(int argc, char **argv) {

  pthread_t             thread;
  t_camera              *camera;
  char                  *device;
  SOCKET                sock;
  SOCKADDR_IN           sin;
  int                   ret;

  if (argc < 3)
    device = "/dev/video1";
  else
    device = argv[2];

  sin = get_server_ip();
  STEP("Create TCP socket");
  sin.sin_port = htons(STREAM_PORT);
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET)
    exit_failure("INVALID_SOCKET");
  VALIDATE();

  STEP("Connection to the server");
  ret = connect(sock, (SOCKADDR*)&sin, sizeof(sin));
  if (ret == SOCKET_ERROR)
    exit_failure("connect");
  VALIDATE();

  camera = open_device(device, 1280, 720);
  camera->sock = &sock;
  get_infos(sock, camera);
  init_device(camera);
  start_camera(camera);

  if(pthread_create(&thread, NULL, listen_thread, camera) == -1)
    exit_failure("pthread_create");

  //  stream_video(camera);
   camera_loop(camera, sock);

  if (pthread_join(thread, NULL))
    exit_failure("pthread_join");

  close(sock);
  stop_camera(camera);
  return (0);
}
