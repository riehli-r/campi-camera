#include "campi/webcam.h"

void                send_image(time_t timestamp, SOCKET sock, t_camera *camera) {

  FILE              *file;
  char              pathname[20];
  int               filesize;
  char              *image;
  char              *base64;
  char              *request;
  size_t            i;
  size_t            base64size;

  memset(pathname, '\0', 20);
  sprintf(pathname, "photos/%ld.jpg", timestamp);
  file = fopen(pathname, "r");
  if (file) {
    printf("File %s is open\n", pathname);
    filesize = fsize(file);
    image = calloc(filesize + 1, sizeof(char));
    fread(image, 1, filesize, file);
    base64 = base64_encode(image, filesize);
    i = 0;
    request = calloc(BUFF_SIZE + 6, sizeof(char));
    base64size = strlen(base64);
    memcpy(request, "image#start", 11);
    camera->is_sending = 1;
    send(sock, request, 11, 0);
    memset(request, '\0', BUFF_SIZE + 6);
    printf("Mutex bloqué\n");
    while (i < base64size) {
      memcpy(request, "image#", 6);
      strncat(request, &base64[i], BUFF_SIZE);
      send(sock, request, BUFF_SIZE + 6, 0);
      memset(request, '\0', BUFF_SIZE + 6);
      recv(sock, request, BUFF_SIZE, 0);
      memset(request, '\0', BUFF_SIZE + 6);
      i += BUFF_SIZE;
    }
    reset_buffer(request);
    memcpy(request, "image#end", 9);
    send(sock, request, 9, 0);
    camera->sock = &sock;
    pthread_cond_signal(&camera->cond);
    fclose(file);
    free(image);
    free(request);
    free(base64);
    printf("Fermeture du fichier\n");
  }
}

void                    camera_loop(t_camera *camera, SOCKET sock) {

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
      rgb = yuyv_to_rgb(camera->head.start, WIDTH, HEIGHT);
      diff = cmp_rgb(camera->prev, rgb, WIDTH, HEIGHT, CONTINUE_ON_LMT);
      if ((diff * 100) /  (WIDTH * HEIGHT) >= camera->infos.precision) {
        printf("precision: %f\n", camera->infos.precision);
        printf("Move: %ld || %f\n", time(NULL), (diff * 100.f) /  (WIDTH * HEIGHT));
        save_current_jpeg(rgb, WIDTH, HEIGHT);
        send_image(time(NULL), sock, camera);
      }
      memcpy(camera->prev, rgb, WIDTH * HEIGHT * 3);
      last_time = time(NULL);
    }
  }
}
