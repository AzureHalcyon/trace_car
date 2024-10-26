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


#endif // !__SENSOR_H__