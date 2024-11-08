#include "pid.h"

float error;
float error_preload;
float kp = 10, ki = 0, kd = 110;
float pre_error = 0;
float integral = 0;
float derivative = 0;
float output = 0;
int turn_left_count = 0;
int turn_right_count = 0;

float LinerPID(void)
{

    error_preload = (850 * normalized_sensors[0] + 230 * normalized_sensors[1] - 230 * normalized_sensors[3] - 850 * normalized_sensors[4]); // 偏差计算，可根据具体需求调整
    if (error_preload > 0)
    {
        error = error_preload - 80 * normalized_sensors[2];
    }
    else
    {
        error = error_preload + 80 * normalized_sensors[2];
    }
    integral += error;
    derivative = error - pre_error;
    output = kp * error + ki * integral + kd * derivative;
    pre_error = error;
    return output;
}

float RoundPID(void)
{
    error = (150 * normalized_sensors[0] - 150 * normalized_sensors[4]); // 偏差计算，可根据具体需求调整
    integral += error;
    derivative = error - pre_error;
    output = kp * error + ki * integral + kd * derivative;
    pre_error = error;
    return output;
}
