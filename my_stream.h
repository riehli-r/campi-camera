#ifndef __MY_STREAM_H__
#define __MY_STREAM_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <jpeglib.h>

#define QUALITY         100

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
int                     camera_frame(t_camera* camera, struct timeval timeouts);
uint8_t*                yuyv_to_rgb(uint8_t* yuyv, uint32_t width, uint32_t height);
void                    write_jpeg_file(FILE *out, uint8_t *rgb, t_camera *camera);

#endif
