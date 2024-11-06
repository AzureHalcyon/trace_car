#include "sensor.h"

int sensors[5];
int threshold_white[5] = {0, 0, 0, 0, 0}; // ÿ���������Ĵ�����ֵ
int threshold_black[5] = {20, 20, 35, 20, 20}; // ÿ���������Ĵ�����ֵ
float normalized_sensors[5];
int L1 = 0, L2 = 0, M = 0, R1 = 0, R2 = 0;
int white_threshold = 0.1; // ������ֵ
int black_threshold = 0.8; // ������ֵ
float sum_left , sum_right, sum_middle; // ��������Ĵ�������
// float left_round = 0, right_round = 0; // ���Ҵ������ĺ�(���ڻ���)

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

//    system_delay_ms(100);
    
    printf("sensors: ");
    printf("%d,",sensors[0]);
    printf("%d,",sensors[1]);
    printf("%d,",sensors[2]);
    printf("%d,",sensors[3]);
    printf("%d\n",sensors[4]);

    //�������Ĵ�����С�ڴ��ף������봿�׵Ĳ�ֵ����PWM,�м�������ڴ��ڣ�ͬ��
    //˼·��ÿ������������ֵ������ͬ���Ƚ��е�λ����Ȼ����ͣ����ݺ͵Ĵ�С������PWM�����ò�������

    // ��λ��������������ֵӳ�䵽0~1֮�䣬0Ϊ���ף�1Ϊ����
    for (int i = 0; i < 5; i++) {
        normalized_sensors[i] = (float)(threshold_white[i] - sensors[i]) / (threshold_white[i] - threshold_black[i]);
        if (normalized_sensors[i] < 0.08) {//��ֹ���������
            normalized_sensors[i] = 0;
        }
    }
    printf("%f,%f,%f,%f,%f\n",normalized_sensors[0],normalized_sensors[1],normalized_sensors[2],normalized_sensors[3],normalized_sensors[4]);
    printf("%f\n",normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]);
    printf("%f\n",(normalized_sensors[0]+normalized_sensors[1])-(normalized_sensors[3]+normalized_sensors[4]));


    sum_left = normalized_sensors[0]*1.5 + normalized_sensors[1];
    sum_right = normalized_sensors[3] + normalized_sensors[4]*1.5;
    sum_middle = normalized_sensors[2];

    // left_round = normalized_sensors[0] + normalized_sensors[1];
    // right_round = normalized_sensors[3] + normalized_sensors[4];

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

//     printf("normalized_sensors: ");
//     printf("%f,",normalized_sensors[0]);
//     printf("%f,",normalized_sensors[1]);
//     printf("%f,",normalized_sensors[2]);
//     printf("%f,",normalized_sensors[3]);
//     printf("%f\n",normalized_sensors[4]);

//    printf("sum: ");
    // printf("%f,",sum_left);
    // printf("%f,",sum_middle);
    // printf("%f,\n",sum_right);

//    printf("round: ");
//    printf("%f\n,",left_round);
//    printf("%f,",sum_middle);
//    printf("%f\n,",right_round);

}

// ����������ֵ��ֵ��
void BinerySensors(){
    if (normalized_sensors[0] > 0.8) {
        L1 = 1;
    }else{
        L1 = 0;
    }
    if (normalized_sensors[1] > 0.8) {
        L2 = 1;
    }else{
        L2 = 0;
    }
    if (normalized_sensors[2] > 0.8) {
        M = 1;
    }else{
        M = 0;
    }
    if (normalized_sensors[3] > 0.8) {
        R1 = 1;
    }else{
        R1 = 0;
    }
    if (normalized_sensors[4] > 0.8) {
        R2 = 1;
    }else{
        R2 = 0;
    }
}
