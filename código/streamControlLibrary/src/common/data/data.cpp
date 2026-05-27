#include "common/data.h"
#include "drivers/fan.h"

ControlData g_data = {
    .temperature = 0.0f,
    .setpoint = 35.0f,
    .kp = 5.0f,
    .ki = 2.5f,
    .kd = 1.2f,
    .integral = 0.0f,
    .last_error = 0.0f,
    .power_percent = 0.0f,
    .fan_speed = FAN_SPEED_NORMAL,   // Flujo de aire base para homogeneización
    .zc_period_us = 0,
    .zc_count = 0,
    .running = false,
    .pid_active = false,
    .sensor_valid = false
};