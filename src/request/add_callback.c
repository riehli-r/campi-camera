#include "request.h"

void                   add_callback(rq_client *client, char *event, void(*callback)()) {

  client->nbr_callback++;
  client->callbacks = realloc(client->callbacks, client->nbr_callback * sizeof(rq_callback));
  client->callbacks[client->nbr_callback - 1].callback = callback;
  client->callbacks[client->nbr_callback - 1].event = event;
}
