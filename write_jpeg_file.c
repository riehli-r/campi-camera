#include "my_stream.h"

void                    write_jpeg_file(FILE *out, uint8_t *rgb, t_camera *camera) {

  JSAMPARRAY            img;
  size_t                i;
  size_t                j;
  struct jpeg_compress_struct compress;
  struct jpeg_error_mgr error;

  img = calloc(camera->height, sizeof (JSAMPROW));
  for (i = 0; i < camera->height; i++) {
    img[i] = calloc(camera->width * 3, sizeof(JSAMPLE));
    for (j = 0; j < camera->width; j++) {
      img[i][j * 3 + 0] = rgb[(i * camera->width + j) * 3 + 0];
      img[i][j * 3 + 1] = rgb[(i * camera->width + j) * 3 + 1];
      img[i][j * 3 + 2] = rgb[(i * camera->width + j) * 3 + 2];
    }
  }

  compress.err = jpeg_std_error(&error);
  jpeg_create_compress(&compress);
  jpeg_stdio_dest(&compress, out);

  compress.image_width = camera->width;
  compress.image_height = camera->height;
  compress.input_components = 3;
  compress.in_color_space = JCS_RGB;
  jpeg_set_defaults(&compress);
  jpeg_set_quality(&compress, QUALITY, TRUE);
  jpeg_start_compress(&compress, TRUE);
  jpeg_write_scanlines(&compress, img, camera->height);
  jpeg_finish_compress(&compress);
  jpeg_destroy_compress(&compress);

  for (i = 0; i < camera->height; i++)
    free(img[i]);

  free(img);
}
