#include "request.h"

rq_req                  send_rq(rq_client *client, rq_req req) {

  char                   *buffer;
  rq_req                 recv_req;

  buffer = client->buffer.data;
  flush(buffer);
  strcat(buffer, req.event);
  strcat(buffer, "#");
  strcat(buffer, req.data);
  client->buffer.length = send(client->sock, buffer, strlen(buffer), 0);
  if (client->buffer.length == SEND_ERROR)
    exit(-1);
  recv_req = recv_rq(client, 1);
  return (recv_req);
}
