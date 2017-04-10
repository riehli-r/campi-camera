#include "request.h"

void                    get_callback(rq_client *client, rq_req *req) {

  unsigned int          i;

  for (i = 0; i < client->nbr_callback; i++) {
    if (!strcmp(client->callbacks[i].event, req->event)) {
      client->callbacks[i].callback(client->param, req->data);
    }
  }
}
