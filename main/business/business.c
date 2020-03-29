#include "business.h"

#define STD_LOCAL_LOG_LEVEL STD_LOG_DEBUG
#define TASK_SIZE 2048
#define TASK_PRI ESP_TASK_MAIN_PRIO

void business_loop(void *argc)
{
    for(;;)
    {
        char *recv = wait_http_req(portMAX_DELAY);
        if(recv == NULL)
            continue;
        STD_LOGD("recv req[%s]", recv);

        char *text;
        json_parse_object(recv, "Text", &text);
        xfs_speak(text);
        json_free(text, 0);
        free_http_req(recv);
    }
}

void business_init()
{
    http_server_init();
    xfs_init();
    http_client_init();
    xTaskCreate(business_loop, "business", TASK_SIZE, NULL, TASK_PRI, NULL);
}