#include "campi/webcam.h"

void                    get_infos(SOCKADDR_IN sin, t_camera *camera) {

  SOCKET                sock;
  int                   ret;
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

  if (!camera->infos.id || !camera->infos.id[0])
    req_id(sock, camera);
  else
    reco(sock, camera);

  close(sock);
}
