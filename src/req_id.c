#include "campi/webcam.h"
#include "campi/request.h"

void                    req_id(t_camera *camera) {

  char                  *buffer;

  send_rq(&camera->cl, (rq_req) {.event = "req", .data = "id"});
  send_rq(&camera->cl, (rq_req) {.event = "label", .data = camera->infos.label});
  buffer = calloc(BUFF_SIZE, sizeof(char));
  sprintf(buffer, "%d", camera->infos.state);
  send_rq(&camera->cl, (rq_req) {.event = "state", .data = buffer});
  sprintf(buffer, "%f", camera->infos.precision);
  send_rq(&camera->cl, (rq_req) {.event = "precision", .data = buffer});
  free(buffer);
}
