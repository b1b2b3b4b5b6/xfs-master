#include "http_client.h"

#define STD_LOCAL_LOG_LEVEL STD_LOG_DEBUG
#define TASK_SIZE 2048
#define TASK_PRI ESP_TASK_MAIN_PRIO

#define KEY_LEN 100
static void client_loop(void *argc)
{
    static char buf[100];
    static char url[KEY_LEN] = {0};
    if(!std_nvs_is_exist(HB_SERVER_KEY))
    {
        strcpy(url, HB_SERVER);
        std_nvs_save(HB_SERVER_KEY, HB_SERVER, KEY_LEN);
    }

    std_nvs_load(HB_SERVER_KEY, url, KEY_LEN);
    for(;;)
    {
        vTaskDelay(HB_TIME_S * 1000 / portTICK_PERIOD_MS);
        memset(buf, 0, 100);
        char *temp = "HB";
        json_pack_object(buf, "Typ", &temp);
        temp = std_wifi_get_ip();
        json_pack_object(buf, "IP", &temp);
        temp = std_wifi_get_stamac_str();
        json_pack_object(buf, "Mac", &temp);
        temp = std_ota_version();
        json_pack_object(buf, "Version", &temp);
        http_post(HB_SERVER, buf);
    }   
    
}   

void http_client_init()
{
    xTaskCreate(client_loop, "client", TASK_SIZE, NULL, TASK_PRI, NULL);
}