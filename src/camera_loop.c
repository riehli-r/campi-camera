#include "campi/webcam.h"

#include <stdint.h>
#include <stdlib.h>


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static int mod_table[] = {0, 2, 1};

char *base64_encode(char *data, size_t input_length) {

  size_t i;
  size_t j;
  size_t output_length;
  uint32_t octet_a;
  uint32_t octet_b;
  uint32_t octet_c;
  char *encoded_data

  output_length = 4 * ((input_length + 2) / 3);

  encoded_data = calloc(output_length + 1, sizeof(char));
  if (!encoded_data)
    return (NULL);

  for (i = 0, j = 0; i < input_length;) {

    octet_a = i < input_length ? (unsigned char)data[i++] : 0;
    octet_b = i < input_length ? (unsigned char)data[i++] : 0;
    octet_c = i < input_length ? (unsigned char)data[i++] : 0;

    uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
  }

  for (int i = 0; i < mod_table[input_length % 3]; i++)
      encoded_data[output_length - 1 - i] = '=';

  return (encoded_data);
}

int nbr_bytes(FILE *file) {

  int nbr;
  fseek(file, 0, SEEK_END);
  nbr = ftell(file);
  fseek(file, 0, SEEK_SET);
  return (nbr);
}




void                send_image(time_t timestamp, SOCKET sock) {

  FILE              *file;
  char              pathname[20];
  int               filesize;
  char              *image;
  char              *base64;
  char              *request;
  size_t            i;
  size_t            base64size;

  reset_buffer(pathname);
  sprintf(pathname, "photos/%ld.jpg", timestamp);
  file = fopen(pathname, "r");
  if (file) {
    printf("File %s is open\n", pathname);
    filesize = nbr_bytes(file);
    image = calloc(filesize + 1, sizeof(char));
    fread(image, 1, filesize, file);
    base64 = base64_encode(image, filesize);
    i = 0;
    request = calloc(BUFF_SIZE + 6, sizeof(char));
    base64size = strlen(base64);
    while (i < base64size) {
      memcpy(request, "image#", 6);
      strncat(request, &base64[i], BUFF_SIZE);
      send(sock, request, BUFF_SIZE + 6, 0);
      memset(request, '\0', BUFF_SIZE + 6);
      recv(sock, request, BUFF_SIZE, 0);
      memset(request, '\0', BUFF_SIZE + 6);
      i += BUFF_SIZE;
    }
    fclose(file);
  }
}

void                    camera_loop(t_camera *camera) {

  time_t                last_time;
  int                   diff;
  uint8_t               *rgb;
  int                   first_loop;

  last_time = 0;
  first_loop = 1;
  while (1) {
    camera_frame(camera);
    if (first_loop) {
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
      first_loop = 0;
    }
    else if (time(NULL) - last_time >= 10 && camera->infos.state) {
      printf("Compare\n");
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      diff = cmp_rgb(camera->prev, rgb, WIDTH, HEIGHT, CONTINUE_ON_LMT);
      if ((diff * 100) /  (WIDTH * HEIGHT) >= camera->infos.precision) {
        printf("precision: %f\n", camera->infos.precision);
        printf("Move: %ld || %f\n", time(NULL), (diff * 100.f) /  (WIDTH * HEIGHT));
        save_current_jpeg(rgb, WIDTH, HEIGHT);
        camera->is_sending = 1;
        send_image(time(NULL), *camera->sock);
        camera->is_sending = 0;
      }
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
      last_time = time(NULL);
    }
  }
}
