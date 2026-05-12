#ifndef CONTROL_TASK_ZC_TRIAC_TASK_H
#define CONTROL_TASK_ZC_TRIAC_TASK_H

#include <stdint.h>

struct ZcTriacData {
    uint32_t zc_count;
    uint32_t period_us;
    uint16_t power_percent;
    uint16_t fire_delay_us;
    bool running;
};

extern ZcTriacData g_zc_triac_data;

void ZcTriacTask(void* parameter);
void zc_triac_task_init();

#endif