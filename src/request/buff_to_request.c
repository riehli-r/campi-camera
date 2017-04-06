#include "request.h"

t_req                 buff_to_request(char *buffer) {

  t_req               request;
  char                *data;
  int                 i;

  request.event = calloc(100, sizeof(char));
  for (i = 0; i < 100 && buffer[i] != '#'; i++)
    request.event[i] = buffer[i];

  request.data = calloc(100, sizeof(char));
  data = strchr(buffer, '#');
  data++;
  memcpy(request.data, data, strlen(data));
  strcat(request.data, "\0");

  return (request);
}
