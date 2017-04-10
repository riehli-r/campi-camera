#include "request.h"

rq_client                client(int *sock, unsigned short display) {

  rq_client              client;

  client.sock = *sock;
  client.display = display;
  client.buffer.data = calloc(BUFF_SIZE, sizeof(char));
  client.buffer.length = 0;
  client.callbacks = NULL;
  client.nbr_callback = 0;

  return (client);
}
