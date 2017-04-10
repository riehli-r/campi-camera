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
#include <ifaddrs.h>

#include <pthread.h>
#include <jpeglib.h>

#include <errno.h>

#include "request.h"

#define DEFAULT_LABEL   "default_label"
#define INIT_FRAME      10
#define RGB_DIFF        20

#define RGB_MAX         255
#define RGB_MIN         0
#define MINMAX(min, max, v) (v > max) ? max : (v < min) ? min : v;
#define PIXEL_COLOR_DIFF(a, b) (((a > b) ? a - b : b - a) > RGB_DIFF)
#define QUALITY         100

#define RETURN_ON_LMT   1
#define CONTINUE_ON_LMT 0
#define BUFF_SIZE       1200
#define DGRAM_PORT      8082
#define STREAM_PORT     8080
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#define SEND_ERROR      -1
#define RECV_ERROR      -1

#define STEP(s)         printf("%s:", s);
#define VALIDATE()      printf("\033[32mOK\033[0m\n");

typedef int             SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef unsigned short  u_short;


typedef struct {
  char                  *event;
  void                  (*action)();
}                       t_action;

typedef struct {
  char                  *id;
  char                  *label;
  unsigned short        state;
  float                 precision;
  unsigned short        stream;
}                       t_info;

typedef struct          s_buffer {
  uint8_t               *start;
  size_t                length;
}                       t_buffer;

typedef struct          s_camera {

  t_info                infos;
  int                   fd;
  SOCKET                *sock;
  uint32_t              width;
  uint32_t              height;
  size_t                buffer_count;
  t_buffer              *buffers;
  t_buffer              head;
  uint8_t               *prev;
  u_short               is_sending;
  u_short               take_picture;
  struct timeval        timeout;
  pthread_mutex_t       mutex;
  pthread_cond_t        cond;
  rq_client             cl;
}                       t_camera;

typedef struct {
  uint8_t               r;
  uint8_t               g;
  uint8_t               b;
}                       t_color;

void                    exit_failure(const char *e);
int                     multi_ioctl(int fd, unsigned long request, void* arg);
int                     fsize(FILE *file);
char*                   base64_encode(char *data, size_t input_length);
uint8_t*                yuyv_to_rgb(uint8_t* yuyv, uint32_t width, uint32_t height);

/** ================= NETWORK FUNCTIONS ================= **/
SOCKADDR_IN             get_server_ip();
void                    get_infos(SOCKET sock, t_camera *camera);
void                    save_id(char *id);
char*                   get_id();
void                    req_id(t_camera *camera);
void                    reco(SOCKET sock, t_camera *camera);
void                    reset_buffer(char *buffer);
void*                   listen_thread(void *camera);
void                    set_id(void* camera, char *data);
void                    set_label(void* camera, char *data);
void                    set_state(void* camera, char *data);
void                    set_precision(void* camera, char *data);
void                    send_image(char* filename, SOCKET sock, t_camera *camera);
void                    send_frame(SOCKET sock, t_buffer head);

/** ================= CAMERA FUNCTIONS ================= **/
t_camera*               open_device(const char* dev, uint32_t width, uint32_t height, SOCKET sock);
void                    capability_requests(t_camera *camera);
void                    formarq_request(t_camera *camera);
void                    buffer_request(t_camera *camera);
void                    init_device(t_camera *camera);
void                    start_camera(t_camera *camera);
void                    stop_camera(t_camera *camera);
void                    camera_loop(t_camera *camera, SOCKET sock);
int                     camera_capture(t_camera *camera);
int                     camera_frame(t_camera* camera);
void                    write_jpeg_file(int out, t_camera *camera);
char*                   save_current_jpeg(uint8_t* rgb, uint32_t width, uint32_t height, char *filename);
void                    take_picture(void *camera, char *data);

/** ================= MOVEMENTS ================= **/
void                    set_color(uint8_t *rgb, t_color *color);
int                     cmp_color(t_color a, t_color b);
int                     cmp_rgb(uint8_t *rgb1, uint8_t *rgb2, uint32_t width, uint32_t height, short limit, float precision);

/** ================= DEV TOOLS ================= **/
void                    display_jpg(t_buffer jpg);
void                    display_rgb(uint8_t *rgb, uint32_t width, uint32_t height);

#endif
