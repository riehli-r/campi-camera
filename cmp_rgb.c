#include "my_stream.h"

int                     cmp_rgb(uint8_t *rgb1, uint8_t *rgb2, uint32_t width, uint32_t height, short limit) {

   int               i;
   t_color           color1;
   t_color           color2;
   int               diff;

   diff = 0;
   for (i = 0; i < width * height * 3; i += 3) {
     set_color(&rgb1[i], &color1);
     set_color(&rgb2[i], &color2);
    //  printf("Difference(%d, %d, %d)\n", color1.r, color1.g, color1.b);
    //  printf("          (%d, %d, %d)\n", color2.r, color2.g, color2.b);
     if (cmp_color(color1, color2)) {
      diff++;
     }
     if (diff >= PIXEL_DIFF && limit) {
       return (diff);
     }
   }

   return (diff);
}
