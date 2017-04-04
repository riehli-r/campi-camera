#include "campi/webcam.h"

char*                    send_request(SOCKET sock, t_req request) {

  char                   *buffer;
  int                    ret;

  buffer = calloc(BUFF_SIZE, sizeof(char));
  strcat(buffer, request.event);
  strcat(buffer, "#");
  strcat(buffer, request.data);
  ret = send(sock, buffer, strlen(buffer), 0);
  if (ret == SEND_ERROR)
    exit_failure("send");
  memset(buffer, '\0', BUFF_SIZE);
  ret = recv(sock, buffer, BUFF_SIZE, 0);
  if (ret == RECV_ERROR)
    exit_failure("recv");
  if (!request.return_buffer)
    free(buffer);
  return (buffer);
}
