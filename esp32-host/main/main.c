#include <stdio.h>
#include "wifi-host.h"
#include <driver/gpio.h>


#define GPIO_IN_UP GPIO_NUM_13
#define GPIO_OUT_UP GPIO_NUM_12

#define GPIO_IN_DOWN GPIO_NUM_14
#define GPIO_OUT_DOWN GPIO_NUM_27


int app_main(void)
{
    gpio_set_direction(GPIO_IN_UP, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_OUT_UP, GPIO_MODE_INPUT_OUTPUT);

    gpio_set_direction(GPIO_IN_DOWN, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_OUT_DOWN, GPIO_MODE_INPUT_OUTPUT);

    wifi_start();
    struct sockaddr_in name;
    socklen_t name_len;
    int server_socket = setup_server(1234, 100000, &name, &name_len);
    while (1)
    {
        signed char signal = acquire_signal(server_socket, &name, &name_len);
        if (signal == 1)
        {
            gpio_set_level(GPIO_IN_UP, 1);
            gpio_set_level(GPIO_OUT_UP, 1);
            gpio_set_level(GPIO_IN_DOWN, 0);
            gpio_set_level(GPIO_OUT_DOWN, 0);
        }
        else if (signal == -1)
        {
            gpio_set_level(GPIO_IN_UP, 0);
            gpio_set_level(GPIO_OUT_UP, 0);
            gpio_set_level(GPIO_IN_DOWN, 1);
            gpio_set_level(GPIO_OUT_DOWN, 1);
        }
        else
        {
            gpio_set_level(GPIO_IN_UP, 0);
            gpio_set_level(GPIO_OUT_UP, 0);
            gpio_set_level(GPIO_IN_DOWN, 0);
            gpio_set_level(GPIO_OUT_DOWN, 0);
        }

        printf("%d\n", signal);
    }

    return 0;
}
