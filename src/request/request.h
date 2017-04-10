#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define DISPLAY         1
#define HIDE            0
#define BUFF_SIZE       1200
#define SEND_ERROR      -1
#define RECV_ERROR      -1

typedef struct {
  char                  *event;
  void                  (*callback)();
}                       rq_callback;

typedef struct {
  char                  *data;
  int                   length;
}                       rq_buffer;

typedef struct {

  int                   sock;
  unsigned short        display;
  rq_buffer             buffer;
  rq_callback           *callbacks;
  unsigned int          nbr_callback;
  void                  *param;

}                       rq_client;

typedef struct {
  char                  *event;
  char                  *data;
  unsigned short        return_buffer;
}                       rq_req;

void                    add_callback(rq_client *client, char *event, void(*callback)());
void                    get_callback(rq_client *client, rq_req *req);
rq_client               client(int *sock, unsigned short display);
char*                   send_request(int sock, rq_req request);
int                     recv_request(int sock, char *buffer, short display);
void                    delete_request(rq_req req);
rq_req                  btorq(rq_buffer *buffer);
rq_buffer*              data_to_buffer(char *data, unsigned int length);
rq_req                  buff_to_request(char *buffer);
void                    flush(char *str);
rq_req                  recv_rq(rq_client *client, unsigned short free_req);
rq_req                  send_rq(rq_client *client, rq_req req);
void                    delete_client(rq_client *client);
void                    add_callback(rq_client *client, char *event, void(*callback)());

#endif
