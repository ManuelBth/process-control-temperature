#ifndef CONTROL_DRIVERS_TRIAC_H
#define CONTROL_DRIVERS_TRIAC_H

#include <stdint.h>

void triac_init();
void triac_arm(uint16_t semiciclo_us, uint16_t power_percent);

#endif