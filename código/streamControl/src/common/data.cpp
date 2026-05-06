#include "common/data.h"

ControlData g_data = {
    .temperature = 0.0f,
    .setpoint = 50.0f,
    .kp = 5.0f,
    .ki = 0.5f,
    .kd = 0.2f,
    .integral = 0.0f,
    .last_error = 0.0f,
    .power_percent = 0.0f,
    .triac_delay_us = 10000,
    .zc_period_us = 20000,
    .zc_last_us = 0,
    .zc_event = false,
    .running = false,
    .pid_active = true,
    .sensor_valid = false
};