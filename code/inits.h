#ifndef __INITS_H__
#define __INITS_H__

#include "zf_common_headfile.h"
#include "defines.h"
#include "sensor.h"
#include "encoder.h"
#include "judge.h"
#include "beep.h"
#include "tft180.h"

void init_sensors();
void init_encoders();
void init_motors();
void init_beep();
void init_tft180();

#endif // !__INITS_H__
