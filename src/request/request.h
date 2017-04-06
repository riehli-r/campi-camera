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
  char                  *data;
  u_short               return_buffer;
}                       t_req;

char*                   send_request(int sock, t_req request);
int                     recv_request(int sock, char *buffer, short display);
void                    delete_request(t_req req);
t_req                   buff_to_request(char *buffer);
void                    flush(char *str);

#endif
