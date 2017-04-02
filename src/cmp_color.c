#include "my_stream.h"

int                 cmp_color(t_color a, t_color b) {
  return (PIXEL_COLOR_DIFF(a.r, b.r) ||
          PIXEL_COLOR_DIFF(a.g, b.g) ||
          PIXEL_COLOR_DIFF(a.b, b.b));
}
