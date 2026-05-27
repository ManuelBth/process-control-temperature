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
    float fan_speed;          // PWM ventiladores (20% normal, 100% perturbación)
    uint32_t zc_period_us;
    uint32_t zc_count;
    bool running;
    bool pid_active;
    bool sensor_valid;
};

extern ControlData g_data;

#endif