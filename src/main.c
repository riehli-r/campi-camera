#include "campi/webcam.h"

void                  (*get_action(char *buffer))(t_camera*, char*) {

  t_action            actions[3];
  t_req               req;
  int                 i;
  req = buff_to_request(buffer);
  actions[0] = (t_action){"set-label", &set_label};
  actions[1] = (t_action){"set-state", &set_state};
  actions[2] = (t_action){"set-precision", &set_precision};
  for (i = 0; i < 3; i++) {
    if (!strcmp(actions[i].event, req.event)) {
      delete_request(req);
      return (actions[i].action);
    }
  }

  delete_request(req);
  return (NULL);
}

void*                    listen_thread(void *camera) {

  t_camera               *tmp;
  char                   *buffer;
  void                   (*action)(t_camera*, char*);

  tmp = (t_camera*) camera;
  printf("Start of listen thread\n");
  buffer = malloc(BUFF_SIZE);
  while (1) {
    pthread_mutex_lock(&tmp->mutex);
    recv_request(*tmp->sock, buffer, DISPLAY);
    action = get_action(buffer);
    if (action)
      action(tmp, buffer);
    pthread_mutex_unlock(&tmp->mutex);
  }

  return (NULL);
}

int                     main(int argc, char **argv) {

  pthread_t             thread;
  t_camera              *camera;
  char                  *device;
  SOCKET                sock;
  SOCKADDR_IN           sin;
  int                   ret;

  if (argc < 3)
    device = "/dev/video0";
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

  camera = open_device(device, WIDTH, HEIGHT);
  camera->sock = &sock;
  get_infos(sock, camera);
  init_device(camera);
  start_camera(camera);

  if(pthread_create(&thread, NULL, listen_thread, camera) == -1)
    exit_failure("pthread_create");

  camera_loop(camera);

  if (pthread_join(thread, NULL))
    exit_failure("pthread_join");

  close(sock);
  stop_camera(camera);
  return (0);
}
