#include "request.h"

rq_buffer*              data_to_buffer(char *data, unsigned int length) {
  rq_buffer            *buffer;

  buffer = malloc(sizeof(rq_buffer));
  if (buffer) {
    data = malloc(sizeof(char) * length);
    strcpy(buffer->data, data);
    buffer->length = length;
    return (buffer);
  }

  return (NULL);
}
