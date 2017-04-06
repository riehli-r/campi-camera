#include "request.h"

int                     recv_request(SOCKET sock, char *buffer, short display) {

  int                   ret;

  flush(buffer);
  ret = recv(sock, buffer, BUFF_SIZE, 0);
  if (ret == RECV_ERROR)
    exit(-1);
  if (display)
    printf("%s\n", buffer);
  return (ret);
}
