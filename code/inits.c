#include "inits.h"

void init_sensors(){
    adc_init(Sensor_L1, ADC_8BIT);
    adc_init(Sensor_L2, ADC_8BIT);
    adc_init(Sensor_M, ADC_8BIT);
    adc_init(Sensor_R1, ADC_8BIT);
    adc_init(Sensor_R2, ADC_8BIT);

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

void init_encoders(){

    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);
    encoder_quad_init(TIM3_ENCODER, TIM3_ENCODER_CH1_P02_6, TIM3_ENCODER_CH2_P02_7);

}

void init_motors(){
    gpio_init(STBY, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN2, GPO, 0, GPO_PUSH_PULL);
    gpio_init(BIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(BIN2, GPO, 0, GPO_PUSH_PULL);

    pwm_init(PWMA, 1000, 0);
    pwm_init(PWMB, 1000, 0);
}

void init_beep(){
    gpio_init(BEEP, GPO, 0, GPO_OPEN_DTAIN);
}
