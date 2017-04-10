#include "campi/webcam.h"
#include "campi/request.h"

void                  take_picture(void *camera, char *data) {

  t_camera            *tmp;

  tmp = (t_camera*) camera;
  (void) data;
  tmp->take_picture = 1;
}
