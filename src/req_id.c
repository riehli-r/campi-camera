#include "campi/webcam.h"

void                    req_id(SOCKET sock, t_camera *camera) {

  char                  *buffer;
  t_req                 req;

  buffer = send_request(sock, (t_req) {.event = "req", .data = "id", .return_buffer = 1});
  req = buff_to_request(buffer);
  memcpy(camera->infos.id, req.data, strlen(req.data));
  delete_request(req);
  printf("%s\n", camera->infos.id);
  save_id(camera->infos.id);
  free(buffer);

  buffer = calloc(BUFF_SIZE, sizeof(char));
  send_request(sock, (t_req) {.event = "label", .data = camera->infos.label});
  sprintf(buffer, "%d", camera->infos.state);
  send_request(sock, (t_req) {.event = "state", .data = buffer});
  sprintf(buffer, "%f", camera->infos.precision);
  send_request(sock, (t_req) {.event = "precision", .data = buffer});
  free(buffer);
}
