#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "zf_common_headfile.h"
#include "defines.h"

extern int sensors[5];
extern int threshold_white[5];
extern int threshold_black[5];
extern float normalized_sensors[5];
extern int white_threshold;
extern float sum_left, sum_right, sum_middle;
extern int L1, L2, M, R1, R2;

void get_sensors();
void BinerySensors();

#endif // !__SENSOR_H__
