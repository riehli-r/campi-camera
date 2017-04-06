#include "campi/webcam.h"

time_t                    save_current_jpeg(uint8_t* yuyv, uint32_t width, uint32_t height)
{
  char                  filename[20];
  FILE                  *dest;
  JSAMPARRAY            image;
  struct                jpeg_compress_struct compress;
  struct                jpeg_error_mgr error;
  size_t                i;
  size_t                j;
  uint8_t               *rgb;
  time_t                current_time;

  current_time = time(NULL);
  rgb = yuyv;
  sprintf(filename, "photos/%ld.jpg", current_time);
  dest = fopen(filename, "w");
  image = calloc(height, sizeof (JSAMPROW));
  for (i = 0; i < height; i++) {
    image[i] = calloc(width * 3, sizeof (JSAMPLE));
    for (j = 0; j < width; j++) {
      image[i][j * 3 + 0] = rgb[(i * width + j) * 3 + 0];
      image[i][j * 3 + 1] = rgb[(i * width + j) * 3 + 1];
      image[i][j * 3 + 2] = rgb[(i * width + j) * 3 + 2];
    }
  }

  compress.err = jpeg_std_error(&error);
  jpeg_create_compress(&compress);
  jpeg_stdio_dest(&compress, dest);

  compress.image_width = width;
  compress.image_height = height;
  compress.input_components = 3;
  compress.in_color_space = JCS_RGB;
  jpeg_set_defaults(&compress);
  jpeg_set_quality(&compress, QUALITY, TRUE);
  jpeg_start_compress(&compress, TRUE);
  jpeg_write_scanlines(&compress, image, height);
  jpeg_finish_compress(&compress);
  jpeg_destroy_compress(&compress);

  for (size_t i = 0; i < height; i++)
    free(image[i]);
  free(image);

  fclose(dest);
  return (current_time);
}
