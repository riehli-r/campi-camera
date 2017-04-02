#include "my_stream.h"

void                     set_color(uint8_t *rgb, t_color *color) {
    color->r = rgb[0];
    color->g = rgb[1];
    color->b = rgb[2];
}
