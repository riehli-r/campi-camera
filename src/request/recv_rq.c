#include "request.h"

rq_req                  recv_rq(rq_client *client, unsigned short free_req) {

  rq_req                req;

  flush(client->buffer.data);
  client->buffer.length = recv(client->sock, client->buffer.data, BUFF_SIZE, 0);
  if (client->buffer.length == RECV_ERROR)
    exit(-1);
  if (client->display)
    printf("%s\n", client->buffer.data);

  req = btorq(&client->buffer);
  get_callback(client, &req);
  if (free_req)
    delete_request(req);
  return (req);
}
