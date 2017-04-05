#include "campi/webcam.h"

int                     recv_request(SOCKET sock, char *buffer, short display) {

  int                   ret;

  reset_buffer(buffer);
  printf("SOCKET: %d\n", sock);
  ret = recv(sock, buffer, BUFF_SIZE, 0);
  if (ret == RECV_ERROR)
    exit_failure("recv");
  if (display)
    printf("%s\n", buffer);
  return (ret);
}
