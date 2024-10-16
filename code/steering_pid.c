#include "steering_pid.h"

float Kp = 1.0, Ki = 0.0, Kd = 0.5; // PIDϵ��
float error = 0, last_error = 0, integral = 0;

//����һ���򻯵���ѧģ����ֻ�� x ������бջ����ƣ�y ����ͨ��Ԥ���״̬�������ٶȡ�
//ͨ�� PID ��������������೵�ֵ�ת�ٲ�Ӷ�����ת��İ뾶�ͽǶȡ�

/**
 * @brief ����һ��ƫ��e , ��ʾС������ں��ߵ�ƫ���������ݴ�������λ�ã�����ͨ����Ȩ���������ƫ��
 * 
 * @param sensors ����������
 * @return float ���ؼ�Ȩ���ƫ��
 * @note ���������ƫ�������Ҳ�ƫ����e=2��L2+1��L1-1��R1-2��R2
 */
float calculateError(int *sensors) {
    //���������ƫ�������Ҳ�ƫ��
    return (2 * sensors[0] + sensors[1] - sensors[3] - 2 * sensors[4]);
}

/**
 * @brief ʹ��λ��ʽ PID ������,���ݵ�ǰ������������ֵ��ٶȲ��죬�Ա���С���ں��ߵ��м�
 * 
 * @param error ƫ��
 * @return float ����������
 */
float calculatePID(float error) {

    float P = Kp * error;
    integral += error;

    if (integral > integral_max) integral = integral_max;
    if (integral < -integral_max) integral = -integral_max;

    float I = Ki * integral;
    float D = Kd * (error - last_error);
    last_error = error;
    
    float delta = P + I + D;
    return delta;
}
