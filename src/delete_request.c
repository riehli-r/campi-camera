#include "campi/webcam.h"

void                    delete_request(t_req req) {
  free(req.event);
  free(req.data);
}
