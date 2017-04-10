#include "request.h"

char*                    send_request(int sock, rq_req req) {

  char                   *buffer;
  int                    ret;

  buffer = calloc(BUFF_SIZE, sizeof(char));
  strcat(buffer, req.event);
  strcat(buffer, "#");
  strcat(buffer, req.data);
  ret = send(sock, buffer, strlen(buffer), 0);
  if (ret == SEND_ERROR)
    exit(-1);
  memset(buffer, '\0', BUFF_SIZE);
  ret = recv(sock, buffer, BUFF_SIZE, 0);
  if (ret == RECV_ERROR)
    exit(-1);
  if (!req.return_buffer)
    free(buffer);
  return (buffer);
}
