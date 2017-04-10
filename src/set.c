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

void                    set_state(void* camera, char *data) {

  t_camera              *tmp;

  tmp = (t_camera*)camera;
  tmp->infos.state = !strcmp(data, "1");
}

void                    set_precision(void* camera, char *data) {

  t_camera              *tmp;

  tmp = (t_camera*)camera;
  tmp->infos.precision = (float)atof(data);
}
