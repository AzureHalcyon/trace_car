#include "motor_driver.h"

void motor_driver_init() {
    gpio_init(STBY, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN2, GPO, 0, GPO_PUSH_PULL);
    gpio_init(BIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(BIN2, GPO, 0, GPO_PUSH_PULL);
    pwm_init(PWMA, 1000, 10000);
    pwm_init(PWMB, 1000, 10000);
}

int constrain(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void setMotorPWM(float left_speed, float right_speed) {
    // 限制PWM输出范围
    int pwm_L = constrain((int)(left_speed * SRATIO), MIN_PWM, MAX_PWM);
    int pwm_R = constrain((int)(right_speed * SRATIO), MIN_PWM, MAX_PWM);

    // 输出到电机
    pwm_set_duty(PWMA, pwm_L);
    pwm_set_duty(PWMB, pwm_R);
}