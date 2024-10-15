#ifndef __SENSOR_H
#define __SENSOR_H

#include "zf_common_headfile.h"
#include "private_defines.h"

extern int sensors[5];

typedef struct{
    adc_channel_enum sensor_pin;
    int sensor_input;
    bool sensor_output;
    // int flag;
}sensor_struct;

void read_sensor_init();
bool read_a_sensor(sensor_struct* sensor);
void read_sensor();

#endif // !__SENSOR_H