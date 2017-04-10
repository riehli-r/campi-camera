#include "campi/webcam.h"
#include "campi/request.h"

void                    req_id(SOCKET sock, t_camera *camera) {

  char                  *buffer;

  send_rq(&camera->cl, (rq_req) {.event = "req", .data = "id"});
 
  buffer = calloc(BUFF_SIZE, sizeof(char));
  send_request(sock, (rq_req) {.event = "label", .data = camera->infos.label});
  sprintf(buffer, "%d", camera->infos.state);
  send_request(sock, (rq_req) {.event = "state", .data = buffer});
  sprintf(buffer, "%f", camera->infos.precision);
  send_request(sock, (rq_req) {.event = "precision", .data = buffer});
  free(buffer);
}
