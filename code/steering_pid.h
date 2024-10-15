#ifndef __STEERING_PID_H
#define __STEERING_PID_H

#include "private_defines.h"
#include "zf_common_headfile.h"

float calculateError(int *sensors);
float calculatePID(float error);

#endif // !__STEERING_PID_H
