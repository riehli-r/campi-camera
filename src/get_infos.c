#include "campi/webcam.h"

void                    get_infos(SOCKET sock, t_camera *camera) {

  if (!camera->infos.id || !camera->infos.id[0])
    req_id(camera);
  else
    reco(sock, camera);
}
