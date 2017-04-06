#include "campi/webcam.h"
#include "campi/request.h"

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
