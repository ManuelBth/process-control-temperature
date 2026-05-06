#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include <stdint.h>

struct ControlData {
    float temperature;
    float setpoint;
    float kp;
    float ki;
    float kd;
    float integral;
    float last_error;
    float power_percent;
    uint16_t triac_delay_us;
    uint32_t zc_period_us;
    uint32_t zc_last_us;
    bool zc_event;
    bool running;
    bool pid_active;
    bool sensor_valid;
};

extern ControlData g_data;

#endif