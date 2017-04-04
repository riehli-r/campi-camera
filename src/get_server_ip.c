#include "campi/webcam.h"

SOCKADDR_IN             get_server_ip() {
  SOCKET                sock;
  SOCKADDR_IN           sin;
  SOCKADDR_IN           server_info;
  char                  *buffer;
  int                   ret;
  int                   broadcast;
  unsigned int          size_sockaddr;
  struct ifaddrs        *ifaddress;
  struct ifaddrs        *tmp;
  SOCKADDR_IN           *sin_test;

  STEP("Init broadcast socket");
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == SOCKET_ERROR)
    exit_failure("SOCKET_ERROR");
  VALIDATE();

  STEP("Set socket option");
  broadcast = 1;
  ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
  if (ret == -1)
    exit_failure("setsockopt");
  VALIDATE();

  getifaddrs(&ifaddress);
  for (tmp = ifaddress; tmp; tmp = tmp->ifa_next) {
    sin_test = (SOCKADDR_IN*)tmp->ifa_broadaddr;
    if (sin_test) {

      printf("Send data to: %s\n", inet_ntoa(sin_test->sin_addr));
      sin.sin_addr.s_addr = sin_test->sin_addr.s_addr;
      sin.sin_family = AF_INET;
      sin.sin_port = htons(DGRAM_PORT);
      buffer = "Hello Dude";
      ret = sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR*)&sin, sizeof(sin));
      if (ret == SEND_ERROR)
        exit_failure("SEND_TO");
    }
  }
  free(ifaddress);

  size_sockaddr = sizeof(server_info);
  buffer = calloc(BUFF_SIZE, sizeof(char));
  if (!buffer)
    exit_failure("malloc dgram buffer");
  recvfrom(sock, buffer, BUFF_SIZE, 0, (SOCKADDR*)&server_info, &size_sockaddr);
  if (strcmp(buffer, "Hello back") == 0) {

    printf("One server found: \033[34m%s\033[0m\n", inet_ntoa(server_info.sin_addr));
    printf("Server %s send: %s\n", inet_ntoa(server_info.sin_addr), buffer);
  }
  free(buffer);
  return (server_info);
}
