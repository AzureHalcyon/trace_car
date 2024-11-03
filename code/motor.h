#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "zf_common_headfile.h"
#include "math.h"
#include "defines.h"
#include "encoder.h"
#include "zf_driver_pwm.h"

void set_motor_speed(pwm_channel_enum pwm_channel, int speed);
void limit(int *value);
void speed_division_A(int speed);
void speed_division_B(int speed);

#endif // !__MOTOR_H__

