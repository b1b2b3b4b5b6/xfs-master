

#include "std_port_common.h"
#include "std_rdebug.h"
#include "std_reboot.h"
#include "std_ota.h"

#include "config.h"
#include "business.h"

#define STD_LOCAL_LOG_LEVEL STD_LOG_DEBUG

void app_main()
{
    std_nvs_init();
    std_reboot_init();
    std_ota_init(REMOTE_BIN_PATH);
    std_wifi_init();
    std_wifi_connect(SSID, PASSWD);
    std_wifi_await_connect(0);
    std_ota_update();

    business_init();

    for(;;)
    {   
        vTaskDelay(2000/ portTICK_PERIOD_MS);
        PORT_HEAP();
    }

}  

