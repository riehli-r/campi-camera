#include "my_stream.h"

void                    write_jpeg_file(int out, t_camera *camera) {

  if (out >= 0) {
    write(out, camera->head.start, camera->head.length);
    close(out);
  }
}
