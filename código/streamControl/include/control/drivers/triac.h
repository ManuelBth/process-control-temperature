#ifndef CONTROL_DRIVERS_TRIAC_H
#define CONTROL_DRIVERS_TRIAC_H

#include <stdint.h>

void triac_init();
void triac_fire(uint16_t delay_us);

#endif