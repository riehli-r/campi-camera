#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

#define MIN_RGB         0
#define MAX_RGB         255
#define MINMAX(min, max, a) (a < min) ? min : (a > max) ? max : a;

uint8_t*                yuyv_to_rgb(uint8_t* yuyv, uint32_t width, uint32_t height)
{
  uint8_t               *rgb;
  size_t                i;
  size_t                j;
  size_t                index;
  int                   y0;
  int                   u;
  int                   y1;
  int                   v;

  rgb = calloc(width * height * 3, sizeof (uint8_t));
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j += 2) {
      index = i * width + j;
      y0 = yuyv[index * 2 + 0] << 8;
      u = yuyv[index * 2 + 1] - 128;
      y1 = yuyv[index * 2 + 2] << 8;
      v = yuyv[index * 2 + 3] - 128;
      rgb[index * 3 + 0] = MINMAX(MIN_RGB, MAX_RGB, (y0 + 359 * v) >> 8);
      rgb[index * 3 + 1] = MINMAX(MIN_RGB, MAX_RGB, (y0 + 88 * v - 183 * u) >> 8);
      rgb[index * 3 + 2] = MINMAX(MIN_RGB, MAX_RGB, (y0 + 454 * u) >> 8);
      rgb[index * 3 + 3] = MINMAX(MIN_RGB, MAX_RGB, (y1 + 359 * v) >> 8);
      rgb[index * 3 + 4] = MINMAX(MIN_RGB, MAX_RGB, (y1 + 88 * v - 183 * u) >> 8);
      rgb[index * 3 + 5] = MINMAX(MIN_RGB, MAX_RGB, (y1 + 454 * u) >> 8);
    }
  }
  return (rgb);
}
