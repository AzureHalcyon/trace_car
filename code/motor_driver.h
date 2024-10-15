#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H

#include "private_defines.h"
#include "zf_common_headfile.h"

void motor_driver_init();
void setMotorPWM(float left_speed, float right_speed);

#endif // !__MOTOR_DRIVER_H