#include "sensor.h"

//ÒÔ×î×ó²àÎª0ºÅ
int sensors[5];

sensor_struct sensor_L2={0};
sensor_struct sensor_L1={0};
sensor_struct sensor_M={0};
sensor_struct sensor_R1={0};
sensor_struct sensor_R2={0};

void read_sensor_init(){
    adc_init(Sensor_L2, ADC_8BIT);
    adc_init(Sensor_L1, ADC_8BIT);
    adc_init(Sensor_M, ADC_8BIT);
    adc_init(Sensor_R1, ADC_8BIT);
    adc_init(Sensor_R2, ADC_8BIT);
    sensor_L2.sensor_pin=Sensor_L2;
    sensor_L1.sensor_pin=Sensor_L1;
    sensor_M.sensor_pin=Sensor_M;
    sensor_R1.sensor_pin=Sensor_R1;
    sensor_R2.sensor_pin=Sensor_R2;
}

bool read_a_sensor(sensor_struct* sensor){
    sensor->sensor_input=adc_mean_filter_convert(sensor->sensor_pin, 5);
    sensor->sensor_output=(sensor->sensor_input>30)?0:1 ;
    return sensor->sensor_output;
}

void read_sensor(){
    sensors[0] == read_a_sensor(&sensor_L2);
    sensors[1] == read_a_sensor(&sensor_L1);
    sensors[2] == read_a_sensor(&sensor_M);
    sensors[3] == read_a_sensor(&sensor_R1);
    sensors[4] == read_a_sensor(&sensor_R2);
    // printf("%d,%d,%d,%d,%d\n",sensor_L2.sensor_output,sensor_L1.sensor_output,sensor_M.sensor_output,sensor_R1.sensor_output,sensor_R2.sensor_output);
}
