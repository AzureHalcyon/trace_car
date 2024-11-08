#include "sensor.h"

int sensors[5];
int threshold_white[5] = {0, 0, 0, 0, 0}; // 每个传感器的纯白阈值
int threshold_black[5] = {305, 335, 375, 335, 305}; // 每个传感器的纯黑阈值
float normalized_sensors[5];
int white_threshold = 0.1; // 纯白阈值
int black_threshold = 0.8; // 纯黑阈值

void get_sensors() {
    sensors[0] = adc_mean_filter_convert(Sensor_L2, 10);//纯白大约50~53，纯黑大约17~19
    sensors[1] = adc_mean_filter_convert(Sensor_L1, 10);//纯白大约62~66，纯黑大约19~21
    sensors[2] = adc_mean_filter_convert(Sensor_M, 10); //纯白大约89~91，纯黑大约32~34（94~98，27~34）
    sensors[3] = adc_mean_filter_convert(Sensor_R1, 10);//纯白大约61~65，纯黑大约18~20（纯白也有68~70）
    sensors[4] = adc_mean_filter_convert(Sensor_R2, 10);//纯白大约64~68，纯黑大约19~22


    // 限制读取到的数值最大为对应传感器的纯白阈值
    for (int i = 0; i < 5; i++) {
        if (sensors[i] > threshold_white[i]) {
            sensors[i] = threshold_white[i];
        }
    }
    
    //如果两侧的传感器小于纯白，根据与纯白的差值调整PWM,中间如果大于纯黑，同理。
    //思路：每个传感器的阈值有所不同，先进行单位化，然后求和，根据和的大小，调整PWM，采用差速驱动
    // 单位化，将传感器的值映射到0~1之间，0为纯白，1为纯黑
    for (int i = 0; i < 5; i++) {
        normalized_sensors[i] = (float)(threshold_white[i] - sensors[i]) / (threshold_white[i] - threshold_black[i]);
        if (normalized_sensors[i] < 0.08) {//防止传感器误差
            normalized_sensors[i] = 0;
        }
    }
    printf("%f\n",normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]);


    if (normalized_sensors[0] >  0.8) {
        gpio_set_level(LED1,0);
    }else{
        gpio_set_level(LED1,1);
    }
    if (normalized_sensors[1] >  0.8) {
        gpio_set_level(LED2,0);
    }else{
        gpio_set_level(LED2,1);
    }
    if (normalized_sensors[3] >  0.8) {
        gpio_set_level(LED3,0);
    }else{  
        gpio_set_level(LED3,1);
    }
    if (normalized_sensors[4] >  0.8) {
        gpio_set_level(LED4,0);
    }else{  
        gpio_set_level(LED4,1);
    }

}
