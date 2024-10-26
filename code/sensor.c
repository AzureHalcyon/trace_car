#include "sensor.h"

int sensors[5];
int threshold_white[5] = {53, 64, 93, 70, 64}; // ÿ���������Ĵ�����ֵ
int threshold_black[5] = {19, 21, 34, 20, 22}; // ÿ���������Ĵ�����ֵ
float normalized_sensors[5];
int white_threshold = 0.9; // ������ֵ
float sum_left , sum_right, sum_middle;

void get_sensors() {
    sensors[0] = adc_mean_filter_convert(Sensor_L2, 10);//���״�Լ50~53�����ڴ�Լ17~19
    sensors[1] = adc_mean_filter_convert(Sensor_L1, 10);//���״�Լ62~66�����ڴ�Լ19~21
    sensors[2] = adc_mean_filter_convert(Sensor_M, 10); //���״�Լ89~91�����ڴ�Լ32~34��94~98��27~34��
    sensors[3] = adc_mean_filter_convert(Sensor_R1, 10);//���״�Լ61~65�����ڴ�Լ18~20������Ҳ��68~70��
    sensors[4] = adc_mean_filter_convert(Sensor_R2, 10);//���״�Լ64~68�����ڴ�Լ19~22

    // ���ƶ�ȡ������ֵ���Ϊ��Ӧ�������Ĵ�����ֵ
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

    //�������Ĵ�����С�ڴ��ף������봿�׵Ĳ�ֵ����PWM,�м�������ڴ��ڣ�ͬ��
    //˼·��ÿ������������ֵ������ͬ���Ƚ��е�λ����Ȼ����ͣ����ݺ͵Ĵ�С������PWM�����ò�������

    // ��λ��

    for (int i = 0; i < 5; i++) {
        normalized_sensors[i] = (float)(threshold_white[i] - sensors[i]) / (threshold_white[i] - threshold_black[i]);
    }

    sum_left = normalized_sensors[0]*3 + normalized_sensors[1];
    sum_right = normalized_sensors[3] + normalized_sensors[4]*3;
    sum_middle = normalized_sensors[2];

//    printf("normalized_sensors: ");
//    printf("%f,",normalized_sensors[0]);
//    printf("%f,",normalized_sensors[1]);
//    printf("%f,",normalized_sensors[2]);
//    printf("%f,",normalized_sensors[3]);
//    printf("%f\n",normalized_sensors[4]);
//
    // printf("sum: ");
    printf("%f,",sum_left);
    // printf("%f,",sum_middle);
    printf("%f,",sum_right);
}