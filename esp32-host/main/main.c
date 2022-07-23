#include <stdio.h>
#include "wifi-host.h"
#include <driver/gpio.h>


#define GPIO_00 GPIO_NUM_26
#define GPIO_01 GPIO_NUM_25

#define GPIO_10 GPIO_NUM_33
#define GPIO_11 GPIO_NUM_32

#define CURRENT 1
#define NO_CURRENT 0


int app_main(void)
{
    wifi_start();
    struct sockaddr_in name;
    socklen_t name_len;
    int server_socket = setup_server(1234, 300000, &name, &name_len);

    gpio_set_direction(GPIO_00, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_01, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_10, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_11, GPIO_MODE_OUTPUT);

    while (1)
    {
        signed char signal = acquire_signal(server_socket, &name, &name_len);
        if (signal == 1)
        {
            gpio_set_level(GPIO_00, CURRENT);
            gpio_set_level(GPIO_01, CURRENT);
            gpio_set_level(GPIO_10, NO_CURRENT);
            gpio_set_level(GPIO_11, NO_CURRENT);
        }
        else if (signal == -1)
        {
            gpio_set_level(GPIO_00, NO_CURRENT);
            gpio_set_level(GPIO_01, NO_CURRENT);
            gpio_set_level(GPIO_10, CURRENT);
            gpio_set_level(GPIO_11, CURRENT);
        }
        else
        {
            gpio_set_level(GPIO_00, NO_CURRENT);
            gpio_set_level(GPIO_01, NO_CURRENT);
            gpio_set_level(GPIO_10, NO_CURRENT);
            gpio_set_level(GPIO_11, NO_CURRENT);
        }

        printf("%d\n", signal);
    }

    return 0;
}
