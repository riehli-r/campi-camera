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
