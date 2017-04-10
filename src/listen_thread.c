#include "campi/webcam.h"
#include "campi/request.h"

void*                    listen_thread(void *camera) {

  t_camera               *tmp;

  tmp = (t_camera*) camera;
  while (1) {
    pthread_mutex_lock(&tmp->mutex);
    if (tmp->is_sending) {
      pthread_cond_wait(&tmp->cond, &tmp->mutex);
      tmp->is_sending = 0;
    }
    recv_rq(&tmp->cl, 1);
    pthread_mutex_unlock(&tmp->mutex);
  }

  return (NULL);
}
