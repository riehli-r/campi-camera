#include "request.h"

rq_req                 btorq(rq_buffer *buffer) {

  rq_req               request;
  char                 *data;
  int                  i;

  request.event = calloc(100, sizeof(char));
  for (i = 0; i < buffer->length && buffer->data[i] != '#'; i++)
    request.event[i] = buffer->data[i];

  data = strchr(buffer->data, '#');
  if (data) {
    request.data = calloc(100, sizeof(char));
    data++;
    memcpy(request.data, data, strlen(data));
    strcat(request.data, "\0");
  }
  else
    request.data = NULL;

  return (request);
}
