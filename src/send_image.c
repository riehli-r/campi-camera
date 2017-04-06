#include "campi/webcam.h"

void                send_image(char *filename, SOCKET sock, t_camera *camera) {

  FILE              *file;
  char              pathname[100];
  int               filesize;
  char              *image;
  char              *base64;
  char              *request;
  size_t            i;
  size_t            base64size;

  memset(pathname, '\0', 100);
  sprintf(pathname, "/home/campie_camera/photos/%s.jpg", filename);
  file = fopen(pathname, "r");
  if (file) {
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
    free(image);
    free(request);
    free(base64);
    fclose(file);
  }
  camera->take_picture = 0;
}
