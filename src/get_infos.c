#include "campi/webcam.h"


void                  save_id(char *buffer) {

  char                *id;
  FILE                *file;

  id = strchr(buffer, '#');
  file = fopen("data/id", "w");
  if (file)
    fprintf(file, "%s", ++id);
}

int                   req_id(SOCKET sock, t_camera *camera) {

  char                *buffer;
  int                 ret;

  buffer = "req#id";
  ret = send(sock, buffer, strlen(buffer), 0);
  if (ret == SEND_ERROR)
    exit_failure("send");
  buffer = calloc(BUFF_SIZE, sizeof(char));
  ret = recv(sock, buffer, BUFF_SIZE, 0);
  printf("ID is: %s\n", buffer);
  save_id(buffer);
  camera->infos.id = buffer;
  if (ret == RECV_ERROR)
    exit_failure("recv");
  free(buffer);


  buffer = "label#default_label";
  send(sock, buffer, strlen(buffer), 0);
  buffer = calloc(BUFF_SIZE, sizeof(char));
  recv(sock, buffer, BUFF_SIZE, 0);
  free(buffer);

  buffer = "state#1";
  send(sock, buffer, strlen(buffer), 0);
  buffer = calloc(BUFF_SIZE, sizeof(char));
  recv(sock, buffer, BUFF_SIZE, 0);
  free(buffer);

  buffer = "precision#2.5";
  send(sock, buffer, strlen(buffer), 0);
  buffer = calloc(BUFF_SIZE, sizeof(char));
  recv(sock, buffer, BUFF_SIZE, 0);
  free(buffer);
  return (1);
}

int                     reco(SOCKET sock, t_camera *camera) {

  char                  *buffer;
  int                    ret;

  printf("Tentative de reco avec id: %s\n", camera->infos.id);
  buffer = calloc(BUFF_SIZE, sizeof(char));
  memcpy(buffer, "reco#", strlen("reco#"));
  strcat(buffer, camera->infos.id);
  ret = send(sock, buffer, strlen(buffer), 0);
  if (ret == SEND_ERROR)
    exit_failure("send");
  memset(buffer, '\0', BUFF_SIZE);
  ret = recv(sock, buffer, BUFF_SIZE, 0);
  if (!strcmp(buffer, "reco#err"))
    return (req_id(sock, camera));
  else {
    printf("%s\n", buffer);
    memset(buffer, '\0', BUFF_SIZE);
    ret = recv(sock, buffer, BUFF_SIZE, 0);
    printf("%s\n", buffer);
    return (1);
  }
}

void                    get_infos(SOCKADDR_IN sin, t_camera *camera) {

  SOCKET                sock;
  int                   ret;
  STEP("Create TCP socket");
  sin.sin_port = htons(STREAM_PORT);
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET)
    exit_failure("INVALID_SOCKET");
  VALIDATE();

  STEP("Connection to the server");
  ret = connect(sock, (SOCKADDR*)&sin, sizeof(sin));
  if (ret == SOCKET_ERROR)
    exit_failure("connect");
  VALIDATE();


  if (!camera->infos.id)
    req_id(sock, camera);
  else
    reco(sock, camera);

  close(sock);
}
