#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include "std_port_common.h"
#include "std_reboot.h"
#include <esp_http_server.h>
#include "config.h"
#include "std_ota.h"

void http_server_init();
char *wait_http_req(int time_ms);
void free_http_req(char *recv);
#endif