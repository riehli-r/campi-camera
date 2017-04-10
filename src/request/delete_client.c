#include "request.h"

void                    delete_client(rq_client *client) {
  free(client->buffer.data);
}
