#include "campi/webcam.h"

void*                    listen_thread(void *camera) {

  t_camera               *tmp;
  char                   *buffer;
  void                   (*action)(t_camera*, char*);
  SOCKET                 sock;

  tmp = (t_camera*) camera;
  sock = *tmp->sock;
  printf("Start of listen thread\n");
  buffer = calloc(BUFF_SIZE, sizeof(char));
  while (1) {
    pthread_mutex_lock(&tmp->mutex);
    if (tmp->is_sending) {
      pthread_cond_wait(&tmp->cond, &tmp->mutex);
      tmp->is_sending = 0;
    }
    recv_request(sock, buffer, DISPLAY);
    action = get_action(buffer);
    if (action)
      action(tmp, buffer);
    pthread_mutex_unlock(&tmp->mutex);
  }

  return (NULL);
}
