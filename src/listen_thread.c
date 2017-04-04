#include "campi/webcam.h"

void*                    listen_thread(void *camera) {

  t_camera               *tmp;
  char                   *buffer;
  void                   (*action)(t_camera*, char*);

  tmp = (t_camera*) camera;
  printf("Start of listen thread\n");
  buffer = malloc(BUFF_SIZE);
  while (1) {
    pthread_mutex_lock(&tmp->mutex);
    recv_request(*tmp->sock, buffer, DISPLAY);
    action = get_action(buffer);
    if (action)
      action(tmp, buffer);
    pthread_mutex_unlock(&tmp->mutex);
  }

  return (NULL);
}
