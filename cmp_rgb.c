#include "my_stream.h"

int                     cmp_rgb(uint8_t *rgb1, uint8_t *rgb2, uint32_t width, uint32_t height) {

   int               i;
   int               j;
   t_color           color1;
   t_color           color2;
   int               diff;

   diff = 0;
   for (i = 0; i < width * height * 3; i += 3) {
     set_color(&rgb1[i], &color1);
     set_color(&rgb2[i], &color2);
     if (cmp_color(color1, color2)) {
      diff++;
     }
     if (diff >= PIXEL_DIFF) {
       return (diff);
     }
   }

   return (diff);
}
