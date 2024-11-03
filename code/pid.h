#ifndef __PID_H__
#define __PID_H__

#include "zf_common_headfile.h"
#include "encoder.h"
#include "motor.h"
#include "defines.h"
#include "sensor.h"

float LinerPID();
float RoundPID();

#endif // __PID_H__