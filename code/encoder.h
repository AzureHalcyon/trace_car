#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "zf_common_headfile.h"
#include "math.h"
#include "defines.h"

void theta_measure();
float calculate_turn_radians(int left_encoder_pulses, int right_encoder_pulses);

#endif // __ENCODER_H__