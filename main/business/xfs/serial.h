#ifndef SERIAL_H
#define SERIAL_H
#include "config.h"
#include "std_port_common.h"

int serial_init();
void serial_send(char *data, int len);

#endif