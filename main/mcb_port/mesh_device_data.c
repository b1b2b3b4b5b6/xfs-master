#include "mesh_device_data.h"

#define STD_LOCAL_LOG_LEVEL STD_LOG_DEBUG
device_info_raw_t g_device_info_raw;

device_info_raw_t *get_device_raw()
{
    return &g_device_info_raw;
}

void format_device_info(char *json)
{
    char *ip = std_wifi_get_ip();
    json_pack_object(json, "IP", &ip);
}

void device_data_init()
{
    memcpy(g_device_info_raw.mac, std_wifi_get_stamac(), 6);
    g_device_info_raw.raw_len = sizeof(device_info_t);
}