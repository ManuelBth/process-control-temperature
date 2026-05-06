#ifndef CONTROL_DRIVERS_ZERO_CROSS_H
#define CONTROL_DRIVERS_ZERO_CROSS_H

#include <stdint.h>

void zc_init();
bool zc_detected();
uint32_t zc_get_period_us();

extern volatile bool g_zc_event;
extern volatile uint32_t g_zc_period_us;
extern volatile uint32_t g_zc_last_us;

#endif