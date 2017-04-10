#include "campi/webcam.h"
#include "campi/request.h"

void                    set_id(void* camera, char *data) {

  t_camera              *tmp;

  tmp = (t_camera*)camera;
  memcpy(tmp->infos.id, data, strlen(data));
  save_id(tmp->infos.id);
}

void                    set_label(void* camera, char *data) {

  t_camera              *tmp;

  tmp = (t_camera*)camera;
  if (tmp->infos.label)
    memcpy(tmp->infos.label, data, strlen(data));
}

void                    set_state(t_camera* camera, char *buffer) {

  rq_req                 req;

  req = buff_to_request(buffer);
  camera->infos.state = !strcmp(req.data, "1");
  delete_request(req);
}

void                    set_precision(t_camera* camera, char *buffer) {

  rq_req                 req;

  req = buff_to_request(buffer);
  camera->infos.precision = (float)atof(req.data);
  delete_request(req);
}
