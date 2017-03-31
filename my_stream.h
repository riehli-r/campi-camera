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

#include <jpeglib.h>

#define INIT_FRAME      5
#define WIDTH           1920
#define HEIGHT          1080
#define RGB_DIFF        20
#define PIXEL_DIFF      100000

#define RGB_MAX         255
#define RGB_MIN         0
#define MINMAX(min, max, v) (v > max) ? max : (v < min) ? min : v;
#define PIXEL_COLOR_DIFF(a, b) (((a > b) ? a - b : b - a) > RGB_DIFF)
#define QUALITY         100

typedef struct {
  uint8_t               r;
  uint8_t               g;
  uint8_t               b;
}                       t_color;

typedef struct          s_buffer {
  uint8_t               *start;
  size_t                length;
}                       t_buffer;

typedef struct          s_camera {

  int                   fd;
  uint32_t              width;
  uint32_t              height;
  size_t                buffer_count;
  t_buffer              *buffers;
  t_buffer              head;
  uint8_t               *prev;
  struct timeval        timeout;
}                       t_camera;

void                    exit_failure(const char *e);
int                     multi_ioctl(int fd, int request, void* arg);

t_camera*               open_device(const char* dev, uint32_t width, uint32_t height);
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
int                     cmp_rgb(uint8_t *rgb1, uint8_t *rgb2, uint32_t width, uint32_t height);

/** ================= DEV TOOLS ================= **/
void                    display_jpg(t_buffer jpg);
void                    display_rgb(uint8_t *rgb, uint32_t width, uint32_t height);

#endif
