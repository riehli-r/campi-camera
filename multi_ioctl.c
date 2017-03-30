#include "my_stream.h"

int                     multi_ioctl(int fd, int request, void* arg) {

  unsigned int          i;
  int                   ret;

  for (i = 0; i < 100; i++) {
    ret = ioctl(fd, request, arg);
    if (ret != -1)
      return (ret);
  }

  return (-1);
}
