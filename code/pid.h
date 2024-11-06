#ifndef __PID_H__
#define __PID_H__

#include "zf_common_headfile.h"
#include "encoder.h"
#include "motor.h"
#include "defines.h"
#include "sensor.h"

float LinerPID();
float RoundPID();

extern int turn_left_count;
extern int turn_right_count;

#endif // __PID_H__
