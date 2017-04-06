#include "campi/webcam.h"

void                    send_frame(SOCKET sock, t_buffer head) {
  char                  *buffer;
  size_t                i;
  char                  *base64;

  base64 = base64_encode((char*)head.start, head.length);
  buffer = calloc(BUFF_SIZE + 6, sizeof(char));
  printf("FRAME SIZE: %ld\n", strlen(base64));
  for (i = 0; i < strlen(base64); i += BUFF_SIZE) {
    memset(buffer, '\0', BUFF_SIZE + 6);
    memcpy(buffer, "frame#", 6);
    strncat(buffer, &base64[i], BUFF_SIZE);
    send(sock, buffer, BUFF_SIZE + 6, 0);
    memset(buffer, '\0', BUFF_SIZE + 6);
    usleep(10500);
    //recv(sock, buffer, BUFF_SIZE + 6, 0);
  }
  memset(buffer, '\0', BUFF_SIZE + 6);
  memcpy(buffer, "frame#end", 9);
  send(sock, buffer, 9, 0);
  free(base64);
  free(buffer);
}
