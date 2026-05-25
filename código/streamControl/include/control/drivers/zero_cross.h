#ifndef CONTROL_DRIVERS_ZERO_CROSS_H
#define CONTROL_DRIVERS_ZERO_CROSS_H

#include <stdint.h>

void zc_init();
bool zc_detected();
void zc_set_power(uint8_t power_percent);
uint32_t zc_get_period_us();
uint32_t zc_get_isr_count();

extern volatile bool g_zc_event;
extern volatile uint32_t g_zc_period_us;
extern volatile uint32_t g_zc_last_us;
extern volatile uint32_t g_zc_isr_count;
extern volatile uint8_t g_zc_last_state;

#endif