#include "sensor.h"

int sensors[5];
int threshold_white[5] = {0, 0, 0, 0, 0}; // 每个传感器的纯白阈值
int threshold_black[5] = {20, 20, 35, 20, 20}; // 每个传感器的纯黑阈值
float normalized_sensors[5];
int white_threshold = 0.9; // 纯白阈值
float sum_left , sum_right, sum_middle; // 三个方向的传感器和
float left_round = 0, right_round = 0; // 左右传感器的和(用于环岛)

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

//   system_delay_ms(100);
    
//    printf("sensors: ");
//    printf("%d,",sensors[0]);
//    printf("%d,",sensors[1]);
//    printf("%d,",sensors[2]);
//    printf("%d,",sensors[3]);
//    printf("%d\n",sensors[4]);

    //如果两侧的传感器小于纯白，根据与纯白的差值调整PWM,中间如果大于纯黑，同理。
    //思路：每个传感器的阈值有所不同，先进行单位化，然后求和，根据和的大小，调整PWM，采用差速驱动

    // 单位化，将传感器的值映射到0~1之间，0为纯白，1为纯黑
    for (int i = 0; i < 5; i++) {
        normalized_sensors[i] = (float)(threshold_white[i] - sensors[i]) / (threshold_white[i] - threshold_black[i]);
    }

    sum_left = normalized_sensors[0]*3 + normalized_sensors[1];
    sum_right = normalized_sensors[3] + normalized_sensors[4]*3;
    sum_middle = normalized_sensors[2];

    left_round = normalized_sensors[0] + normalized_sensors[1];
    right_round = normalized_sensors[3] + normalized_sensors[4];

    // printf("normalized_sensors: ");
    // printf("%f,",normalized_sensors[0]);
    // printf("%f,",normalized_sensors[1]);
    // printf("%f,",normalized_sensors[2]);
    // printf("%f,",normalized_sensors[3]);
    // printf("%f\n",normalized_sensors[4]);

    // printf("sum: ");
    // printf("%f\n,",sum_left);
    // printf("%f,",sum_middle);
    // printf("%f,\n",sum_right);
}
