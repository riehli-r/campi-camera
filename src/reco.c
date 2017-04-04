#include "campi/webcam.h"

void                    set_label(t_camera* camera, char *buffer) {

  t_req                 req;

  req = buff_to_request(buffer);
  if (camera->infos.label)
    memcpy(camera->infos.label, req.data, strlen(req.data));
  delete_request(req);
}

void                    set_state(t_camera* camera, char *buffer) {

  t_req                 req;

  req = buff_to_request(buffer);
  camera->infos.state = !strcmp(req.data, "1");
  delete_request(req);
}

void                    set_precision(t_camera* camera, char *buffer) {

  t_req                 req;

  req = buff_to_request(buffer);
  camera->infos.precision = (float)atof(req.data);
  delete_request(req);
}

void                    reco(SOCKET sock, t_camera *camera) {

  char                  *buffer;

  printf("Tentative de reco avec id: %s\n", camera->infos.id);
  buffer = send_request(sock, (t_req) {.event = "reco", .data = camera->infos.id, .return_buffer = 1});
  if (!strcmp(buffer, "reco#err")) {
    printf("Invalid ID\n");
    req_id(sock, camera);
  }
  else {
    set_label(camera, buffer);
    recv_request(sock, buffer, HIDE);
    set_state(camera,buffer);
    recv_request(sock, buffer, HIDE);
    set_precision(camera, buffer);
    free(buffer);
  }
}
