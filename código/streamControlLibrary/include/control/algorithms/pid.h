#ifndef CONTROL_ALGORITHMS_PID_H
#define CONTROL_ALGORITHMS_PID_H

#include <stdint.h>

void pid_init(float kp, float ki, float kd);
float pid_calculate(float temperature, float setpoint, float dt);
void pid_reset();

#endif