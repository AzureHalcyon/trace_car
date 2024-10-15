#include "steering_pid.h"

// PID ���ƴ���
float Kp = 1.0, Ki = 0.0, Kd = 0.5; // PIDϵ��
float error = 0, last_error = 0, integral = 0;
float base_speed = 100; // �����ٶ�

//����һ��ƫ��e , ��ʾС������ں��ߵ�ƫ���������ݴ�������λ�ã�����ͨ����Ȩ���������ƫ��e=2��L2+1��L1-1��R1-2��R2
//����һ���򻯵���ѧģ����ֻ�� x ������бջ����ƣ�y ����ͨ��Ԥ���״̬�������ٶȡ�
//ͨ�� PID ��������������೵�ֵ�ת�ٲ�Ӷ�����ת��İ뾶�ͽǶȡ�

float calculateError(int *sensors) {
    //���������ƫ�������Ҳ�ƫ��
    return (2 * sensors[0] + sensors[1] - sensors[3] - 2 * sensors[4]);
}

// ����: ��ǰƫ��error
float calculatePID(float error) {
    // ����PID����ֵ
    float P = Kp * error;
    integral += error;
    float I = Ki * integral;
    float D = Kd * (error - last_error);
    last_error = error;
    
    // �ܵĵ�����
    float delta = P + I + D;
    return delta;
    // // �������ֺ������ٶ�
    // float left_speed = base_speed - delta;
    // float right_speed = base_speed + delta;
    
    // setMotorPWM(left_speed, right_speed);
}