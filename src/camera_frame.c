#include "campi/webcam.h"

int                     camera_frame(t_camera* camera) {

    fd_set              fds;
    int                 ret;
    struct timeval      timeout;

    timeout = camera->timeout;
    FD_ZERO(&fds);
    FD_SET(camera->fd, &fds);
    ret = select(camera->fd + 1, &fds, 0, 0, &timeout);
    if (ret == -1)
      exit_failure("select");
    else if (!ret)
      return (0);
    return (camera_capture(camera));
}
