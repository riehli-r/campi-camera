#ifndef __MY_STREAM_H__
#define __MY_STREAM_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <jpeglib.h>

#define INIT_FRAME      10
#define WIDTH           1280
#define HEIGHT          720
#define RGB_DIFF        20
#define PIXEL_DIFF      2.5

#define RGB_MAX         255
#define RGB_MIN         0
#define MINMAX(min, max, v) (v > max) ? max : (v < min) ? min : v;
#define PIXEL_COLOR_DIFF(a, b) (((a > b) ? a - b : b - a) > RGB_DIFF)
#define QUALITY         100

#define RETURN_ON_LMT   1
#define CONTINUE_ON_LMT 0

#define BUFF_SIZE       2048
#define DGRAM_PORT      5656
#define STREAM_PORT     4848
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#define SEND_ERROR      -1
#define RECV_ERROR      -1
#define strlend(s)      strlen(s) + 1

#define STEP(s)         printf("%s:", s);
#define VALIDATE()      printf("\033[32mOK\033[0m\n");

typedef int             SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr  IN_ADDR;

typedef struct {
  char                  *id;
  char                  *label;
  unsigned short        state;
  float                 precision;
}                       t_info;

typedef struct          s_buffer {
  uint8_t               *start;
  size_t                length;
}                       t_buffer;

typedef struct          s_camera {

  t_info                infos;
  int                   fd;
  uint32_t              width;
  uint32_t              height;
  size_t                buffer_count;
  t_buffer              *buffers;
  t_buffer              head;
  uint8_t               *prev;
  struct timeval        timeout;
}                       t_camera;

typedef struct {
  uint8_t               r;
  uint8_t               g;
  uint8_t               b;
}                       t_color;

void                    exit_failure(const char *e);
int                     multi_ioctl(int fd, unsigned long request, void* arg);

t_camera*               open_device(const char* dev, uint32_t width, uint32_t height, char *label);
void                    capability_requests(t_camera *camera);
void                    cropcap_requests(t_camera *camera);
void                    format_request(t_camera *camera);
void                    buffer_request(t_camera *camera);
void                    init_device(t_camera *camera);
void                    start_camera(t_camera *camera);
void                    stop_camera(t_camera *camera);
int                     camera_capture(t_camera *camera);
int                     camera_frame(t_camera* camera);
void                    write_jpeg_file(int out, t_camera *camera);
uint8_t*                yuyv_to_rgb(uint8_t* yuyv, uint32_t width, uint32_t height);
void                    save_current_jpeg(uint8_t* rgb, uint32_t width, uint32_t height);

/** ================= MOVEMENTS ================= **/
void                    set_color(uint8_t *rgb, t_color *color);
int                     cmp_color(t_color a, t_color b);
int                     cmp_rgb(uint8_t *rgb1, uint8_t *rgb2, uint32_t width, uint32_t height, short limit);

/** ================= DEV TOOLS ================= **/
void                    display_jpg(t_buffer jpg);
void                    display_rgb(uint8_t *rgb, uint32_t width, uint32_t height);

#endif
