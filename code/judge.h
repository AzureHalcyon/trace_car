#ifndef __JUDGE_H__
#define __JUDGE_H__

#include "zf_common_headfile.h"
#include "math.h"
#include "defines.h"
#include "sensor.h"
#include "encoder.h"
#include "motor.h"
#include "pid.h"

int LinerDetect();
void rRoundDetect();
void lRoundDetect();
void JudgeThread();

#endif // __JUDGE_H__
