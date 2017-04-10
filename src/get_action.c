#include "campi/webcam.h"
#include "campi/request.h"


void                  take_picture(t_camera *camera, char *buffer) {
  (void) buffer;
  camera->take_picture = 1;
}


void                  (*get_action(char *buffer))(t_camera*, char*) {

  t_action            actions[4];
  rq_req               req;
  int                 i;

  req = buff_to_request(buffer);
  actions[0] = (t_action){"set-label", &set_label};
  actions[1] = (t_action){"set-state", &set_state};
  actions[2] = (t_action){"set-precision", &set_precision};
  actions[3] = (t_action){"take-picture", &take_picture};
  for (i = 0; i < 4; i++) {
    if (!strcmp(actions[i].event, req.event)) {
      delete_request(req);
      return (actions[i].action);
    }
  }

  delete_request(req);
  return (NULL);
}
