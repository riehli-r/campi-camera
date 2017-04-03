#include "campi/webcam.h"

void                    get_infos(SOCKADDR_IN sin) {

  SOCKET                sock;
  char                  *buffer;
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

  STEP("First contact with the server");
  buffer = "test#hello";
  ret = send(sock, buffer, strlend(buffer), 0);
  if (ret == SEND_ERROR)
    exit_failure("send");
  buffer = calloc(BUFF_SIZE, sizeof(char));
  ret = recv(sock, buffer, BUFF_SIZE, 0);
  if (ret == RECV_ERROR)
    exit_failure("recv");
  VALIDATE();
  printf("Server say: %s\n", buffer);

  free(buffer);
  close(sock);
}
