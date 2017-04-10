#include "request.h"

void                    delete_request(rq_req req) {
  if (req.event)
    free(req.event);
  if (req.data)
    free(req.data);
}
