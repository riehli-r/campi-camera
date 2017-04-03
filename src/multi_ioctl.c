#include "campi/webcam.h"

int                     multi_ioctl(int fd, unsigned long request, void* arg) {

  unsigned int          i;
  int                   ret;

  for (i = 0; i < 100; i++) {
    ret = ioctl(fd, request, arg);
    if (ret != -1)
      return (ret);
  }

  return (-1);
}
