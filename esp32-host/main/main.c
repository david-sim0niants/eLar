#include <stdio.h>
#include "wifi-host.h"
#include <driver/gpio.h>


#define GPIO_00 GPIO_NUM_26
#define GPIO_01 GPIO_NUM_25

#define GPIO_10 GPIO_NUM_14
#define GPIO_11 GPIO_NUM_27


int app_main(void)
{
    gpio_set_direction(GPIO_00, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_01, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_10, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_11, GPIO_MODE_OUTPUT);

    wifi_start();
    struct sockaddr_in name;
    socklen_t name_len;
    int server_socket = setup_server(1234, 100000, &name, &name_len);
    while (1)
    {
        signed char signal = acquire_signal(server_socket, &name, &name_len);
        if (signal == 1)
        {
            gpio_set_level(GPIO_00, 1);
            gpio_set_level(GPIO_01, 1);
            gpio_set_level(GPIO_10, 0);
            gpio_set_level(GPIO_11, 0);
        }
        else if (signal == -1)
        {
            gpio_set_level(GPIO_00, 0);
            gpio_set_level(GPIO_01, 0);
            gpio_set_level(GPIO_10, 1);
            gpio_set_level(GPIO_11, 1);
        }
        else
        {
            gpio_set_level(GPIO_00, 0);
            gpio_set_level(GPIO_01, 0);
            gpio_set_level(GPIO_10, 0);
            gpio_set_level(GPIO_11, 0);
        }

        printf("%d\n", signal);
    }

    return 0;
}
