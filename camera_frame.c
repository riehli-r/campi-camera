#include "my_stream.h"

int                     camera_frame(t_camera* camera, struct timeval timeouts) {

    fd_set              fds;
    int                 ret;

    FD_ZERO(&fds);
    FD_SET(camera->fd, &fds);
    ret = select(camera->fd + 1, &fds, 0, 0, &timeouts);
    if (ret == -1)
      exit_failure("select");
    else if (!ret)
      return (0);
    return (camera_capture(camera));
}
