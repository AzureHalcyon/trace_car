#include "inits.h"

void init_sensors(){
    adc_init(Sensor_L1, ADC_8BIT);
    adc_init(Sensor_L2, ADC_8BIT);
    adc_init(Sensor_M, ADC_8BIT);
    adc_init(Sensor_R1, ADC_8BIT);
    adc_init(Sensor_R2, ADC_8BIT);

    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(LED3, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(LED4, GPO, GPIO_HIGH, GPO_PUSH_PULL);

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

    display("sensors inited\n");
}

void init_encoders(){
    encoder_quad_init(TIM2_ENCODER, ENA1, ENB1);
    encoder_quad_init(TIM3_ENCODER, ENA2, ENB2);

    display("encoders inited\n");
}

void init_motors(){
    gpio_init(STBY, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN1, GPO, 0, GPO_PUSH_PULL);
    gpio_init(AIN2, GPO, 1, GPO_PUSH_PULL);
    gpio_init(BIN1, GPO, 0, GPO_PUSH_PULL);
    gpio_init(BIN2, GPO, 1, GPO_PUSH_PULL);

    pwm_init(PWMA, 1000, 0);
    pwm_init(PWMB, 1000, 0);

    display("motors inited\n");
}

void init_beep(){
    gpio_init(BEEP, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    display("beep inited\n");
}

void init_tft180(){
    tft180_init ();
    tft180_clear();
//    tft180_show_string(0, 48, "all_inited");
    display("ready!\n");
}
