#include <stdio.h>
#include "wifi-host.h"


int app_main(void)
{
    wifi_start();
    struct sockaddr_in name;
    socklen_t name_len;
    int server_socket = setup_server(1234, 100000, &name, &name_len);
    while (1)
    {
        char signal = acquire_signal(server_socket, &name, &name_len);
        printf("%hhx\n", signal);
    }

    return 0;
}