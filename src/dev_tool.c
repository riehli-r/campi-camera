#include "my_stream.h"

void                    display_jpg(t_buffer jpg) {
  size_t                i;

  for (i = 0; i < jpg.length; i++) {
    printf("%02x ", jpg.start[i]);
  }
  printf("/n");
}

void                    display_rgb(uint8_t *rgb, uint32_t width, uint32_t height) {
  size_t                i;

  for (i = 0; i < width * height * 3; i++) {
    printf("%02x ", rgb[i]);
  }
  printf("/n");
}
