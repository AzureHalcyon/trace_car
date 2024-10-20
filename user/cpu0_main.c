/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "math.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

#define PWMA                    (ATOM0_CH1_P21_3)
#define PWMB                    (ATOM0_CH0_P21_2)
#define AIN1                    (P22_2)
#define AIN2                    (P22_3)
#define BIN1                    (P22_1)
#define BIN2                    (P22_0)
#define STBY                    (P23_1)
#define ENA                     (P33_7)
#define ENB                     (P33_6)

#define Sensor_L2               (ADC0_CH0_A0)
#define Sensor_L1               (ADC0_CH1_A1)
#define Sensor_M                (ADC0_CH2_A2)
#define Sensor_R1               (ADC0_CH3_A3)
#define Sensor_R2               (ADC0_CH4_A4)

// **************************** �������� ****************************

void turns();
int init_sensors();
// double calculate_turn_radians(int left_encoder_pulses, int right_encoder_pulses);
void limit();

int sensors[5];
int threshold_white[5] = {53, 64, 93, 70, 64}; // ÿ���������Ĵ�����ֵ
int threshold_black[5] = {19, 21, 34, 20, 22}; // ÿ���������Ĵ�����ֵ
float normalized_sensors[5];
int white_threshold = 0.9; // ������ֵ

int base_speed = 3000; // �����ٶ�
int max_speed_diff = 1500; // ����ٶȲ�

// int integral_max = 100;
// float Kp = 3.0, Ki = 0.3, Kd = 0.7; // PIDϵ��
// float error = 0, last_error = 0, integral = 0;

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);
    gpio_init(STBY, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN2, GPO, 0, GPO_PUSH_PULL);
    gpio_init(BIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(BIN2, GPO, 0, GPO_PUSH_PULL);
    adc_init(Sensor_L1, ADC_8BIT);
    adc_init(Sensor_L2, ADC_8BIT);
    adc_init(Sensor_M, ADC_8BIT);
    adc_init(Sensor_R1, ADC_8BIT);
    adc_init(Sensor_R2, ADC_8BIT);
    pwm_init(PWMA, 1000, 0);
    pwm_init(PWMB, 1000, 0);
    init_sensors();
    pit_ms_init(CCU60_CH0 , 25);

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);
    // encoder_get_count(TIM2_ENCODER);
    turns();

}

int init_sensors() {
    int sensor_values[5] = {0};
    for (int i = 0; i < 50; i++) {
        sensor_values[0] += adc_mean_filter_convert(Sensor_L2, 10);
        sensor_values[1] += adc_mean_filter_convert(Sensor_L1, 10);
        sensor_values[2] += adc_mean_filter_convert(Sensor_M, 10);
        sensor_values[3] += adc_mean_filter_convert(Sensor_R1, 10);
        sensor_values[4] += adc_mean_filter_convert(Sensor_R2, 10);
        printf("No.%d times\n",i);
    }

    for (int i = 0; i < 5; i++) {
        threshold_white[i] = sensor_values[i] / 50;
    }

    printf("success!\n");

    printf("threshold_white: ");
    printf("%d,",threshold_white[0]);
    printf("%d,",threshold_white[1]);
    printf("%d,",threshold_white[2]);
    printf("%d,",threshold_white[3]);
    printf("%d\n",threshold_white[4]);

//    printf("sensors: ");
//    printf("%d,",sensors[0]);
//    printf("%d,",sensors[1]);
//    printf("%d,",sensors[2]);
//    printf("%d,",sensors[3]);
//    printf("%d\n",sensors[4]);
}

//�޷���0~10000
int left = 0;
int right = 0;
int leftCompare = 0;
int rightCompare = 0;
void limit(){
    if (left > 10000) {
        leftCompare = 10000;
    } else if (left < 0) {
        leftCompare = 0;
    }else leftCompare = left;

    if (right > 10000) {
        rightCompare = 10000;
    } else if (right < 0) {
        rightCompare = 0;
    }else rightCompare = right;
}

void turns(){
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
    
//    printf("sensors: ");
//    printf("%d,",sensors[0]);
//    printf("%d,",sensors[1]);
//    printf("%d,",sensors[2]);
//    printf("%d,",sensors[3]);
//    printf("%d\n",sensors[4]);

    //�������Ĵ�����С�ڴ��ף������봿�׵Ĳ�ֵ����PWM,�м�������ڴ��ڣ�ͬ��
    //˼·��ÿ������������ֵ������ͬ���Ƚ��е�λ����Ȼ����ͣ����ݺ͵Ĵ�С������PWM�����ò�������

    // ת���߼�

    for (int i = 0; i < 5; i++) {
        normalized_sensors[i] = (float)(threshold_white[i] - sensors[i]) / (threshold_white[i] - threshold_black[i]);
    }

    float sum_left = normalized_sensors[0]*2 + normalized_sensors[1];
    float sum_right = normalized_sensors[3] + normalized_sensors[4]*2;
    float sum_middle = normalized_sensors[2];

//    printf("normalized_sensors: ");
//    printf("%f,",normalized_sensors[0]);
//    printf("%f,",normalized_sensors[1]);
//    printf("%f,",normalized_sensors[2]);
//    printf("%f,",normalized_sensors[3]);
//    printf("%f\n",normalized_sensors[4]);
//
    printf("sum: ");
    printf("%f,",sum_left);
    printf("%f,",sum_middle);
    printf("%f\n",sum_right);

    // �������Ҵ��������ܺ͵���PWM
    int speed_diff = (int)(sum_left - sum_right) * max_speed_diff;

    if (sum_middle > 0.8 && fabs(sum_left - sum_right) < 0.2) {
        pwm_set_duty(PWMA, base_speed);
        pwm_set_duty(PWMB, base_speed);
        printf("straight!\n");
    }else if (0.85 < fabs(sum_left - sum_right) && fabs(sum_left - sum_right) < 1.3) { //Сת��

        left = (base_speed + speed_diff);
        right = (base_speed - speed_diff);
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
        printf("turning!\n");
    }else if (fabs(sum_left - sum_right) > 1.3){ //��ת��

        left = (base_speed + speed_diff) * 1.5;
        right = (base_speed - speed_diff)* 1.5;
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
        printf("turning!\n");
    } 

    // ���ܷɱ���

    if (sum_middle <0.15 && fabs(sum_left - sum_right) < 0.25) {
        // ���д���������⵽���ף�ֹͣС��
        pwm_set_duty(PWMA, 0);
        pwm_set_duty(PWMB, 0);
        printf("stopped!\n");
        return;
    }


}

// #define WHEEL_DISTANCE 0.12  //С��������֮��ľ��룬��λ����
// #define PULSES_PER_REV 1024 //ÿת������������(����)
// #define WHEEL_RADIUS 0.0225 //���ְ뾶����λ����
// #define PULSE_DISTANCE (2 * 3.1415926 * WHEEL_RADIUS / PULSES_PER_REV)//ÿ�������Ӧ�ľ���(����)

// double calculate_turn_radians(int left_encoder_pulses, int right_encoder_pulses) {
//     double delta_left = left_encoder_pulses * PULSE_DISTANCE;
//     double delta_right = right_encoder_pulses * PULSE_DISTANCE;
//     double theta = (delta_right - delta_left) / WHEEL_DISTANCE;
//     return theta;
// }

// void theta_measure() {
//     int left_encoder_pulses = encoder_get_count(TIM2_ENCODER);
//     int right_encoder_pulses = encoder_get_count(TIM2_ENCODER);
//     double theta = calculate_turn_radians(left_encoder_pulses, right_encoder_pulses);
//     printf("theta: %f\n", theta);
// }


#pragma section all restore
// **************************** �������� ****************************
