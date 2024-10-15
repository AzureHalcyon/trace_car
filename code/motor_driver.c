#include "motor_driver.h"

void setMotorPWM(float left_speed, float right_speed) {
    // ����PWM�����Χ
    int pwm_L = constrain((int)(left_speed * k), MIN_PWM, MAX_PWM);
    int pwm_R = constrain((int)(right_speed * k), MIN_PWM, MAX_PWM);

    // ��������
    analogWrite(MOTOR_LEFT_PIN, pwm_L);
    analogWrite(MOTOR_RIGHT_PIN, pwm_R);
}