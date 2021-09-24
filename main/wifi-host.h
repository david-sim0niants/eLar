#ifndef __ESP32_WIFI_HOST_H__
#define __ESP32_WIFI_HOST_H__

#include "lwip/sockets.h"

void wifi_start();
int setup_server(ushort port, int recv_timeout, struct sockaddr_in *name, socklen_t *name_len);
char acquire_signal(int server_socket, struct sockaddr_in *name, socklen_t *name_len);



#endif
