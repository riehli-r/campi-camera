#include "my_stream.h"

SOCKADDR_IN         get_server_ip() {
  SOCKET            sock;
  SOCKADDR_IN       sin;
  SOCKADDR_IN       server_info;
  char              *buffer;
  int               ret;
  int               broadcast;
  unsigned int      size_sockaddr;

  STEP("Init broadcast socket");
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == SOCKET_ERROR)
    exit_failure("SOCKET_ERROR");
  VALIDATE();

  STEP("Set socket option");
  broadcast = 1;
  ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
  if (ret == -1)
    exit_failure("setsockopt");
  VALIDATE();

  STEP("Send request to broadcast");
  sin.sin_addr.s_addr = inet_addr("255.255.255.255");
  sin.sin_family = AF_INET;
  sin.sin_port = htons(DGRAM_PORT);
  buffer = "Hello Dude";
  ret = sendto(sock, buffer, strlen(buffer) + 1, 0, (SOCKADDR*)&sin, sizeof(sin));
  if (ret == SEND_ERROR)
    exit_failure("SEND_TO");
  VALIDATE();

  buffer = calloc(BUFF_SIZE, sizeof(char));
  if (!buffer)
    exit_failure("malloc dgram buffer");

  STEP("Receive date from main server");
  size_sockaddr = sizeof(server_info);
  recvfrom(sock, buffer, sizeof(buffer), 0, (SOCKADDR*)&server_info, &size_sockaddr);
  VALIDATE();
  printf("Server %s send: %s\n", inet_ntoa(server_info.sin_addr), buffer);
  free(buffer);
  return (server_info);
}

void                get_infos(SOCKADDR_IN sin) {

  SOCKET            sock;
  char              *buffer;
  int               ret;

  sin.sin_port = htons(STREAM_PORT);
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET)
    exit_failure("INVALID_SOCKET");

  ret = connect(sock, (SOCKADDR*)&sin, sizeof(sin));
  if (ret == SOCKET_ERROR)
    exit_failure("connect");

  buffer = "More private now";
  ret = send(sock, buffer, strlen(buffer) + 1, 0);
  if (ret == SEND_ERROR)
    exit_failure("send");

  buffer = calloc(BUFF_SIZE, sizeof(char));
  ret = recv(sock, buffer, sizeof(buffer), 0);
  if (ret == RECV_ERROR)
    exit_failure("recv");

  printf("Server say: %s\n", buffer);
  free(buffer);
}

int                 main(int argc, char **argv) {

  t_camera          *camera;
  int               diff;
  char              *device;
  uint8_t           *rgb;
  time_t            last_time;

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
  while(1) {
    camera_frame(camera);
    if (last_time != time(NULL)) {
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
