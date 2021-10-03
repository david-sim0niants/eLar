#include "wifi-host.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include <time.h>


#ifndef HOST_WIFI_SSID
#define HOST_WIFI_SSID "eLar"
#endif

#ifndef HOST_WIFI_PWD
#define HOST_WIFI_PWD "87654321"
#endif

#ifndef HOST_WIFI_AUTHMODE
#define HOST_WIFI_AUTHMODE WIFI_AUTH_WPA2_PSK
#endif



static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)   
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        printf("Mac: %x:%x:%x:%x:%x:%x\n", event->mac[0], event->mac[1], event->mac[2], event->mac[3], event->mac[4], event->mac[5]);
    }
}


void wifi_start()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase()); 
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *wifi_ap = esp_netif_create_default_wifi_ap();

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    wifi_config_t ap_config = {
        .ap = {
            .ssid = HOST_WIFI_SSID,
            .channel = 0,
            .authmode = HOST_WIFI_AUTHMODE,
            .password = HOST_WIFI_PWD,
            .ssid_hidden = 0,
            .max_connection = 2,
            .beacon_interval = 100
        }
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));

    ESP_ERROR_CHECK(esp_wifi_start());
}


int setup_server(ushort port, int recv_timeout, struct sockaddr_in *name, socklen_t *name_len)
{
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    struct timeval timeout = {
        .tv_sec = 0,
        .tv_usec = 100000
    };
    setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    const struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };
    bind(server_socket, (struct sockaddr *)&address, sizeof(address));
    *name = address;
    *name_len = sizeof(address);
    return server_socket;
}


char acquire_signal(int server_socket, struct sockaddr_in *name, socklen_t *name_len)
{
    signed char signal = 0;
    recvfrom(server_socket, &signal, sizeof(char), 0, (struct sockaddr *)name, name_len);
    return signal;
}


void close_server(int server_socket, struct sockaddr_in *name, socklen_t *name_len)
{
    const char close_signal = 1;
    sendto(server_socket, &close_signal, sizeof(char), 0, (struct sockaddr *)name, *name_len);
    closesocket(server_socket);
}