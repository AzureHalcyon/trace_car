#include "judge.h"

int base_speed = 3000;          // �����ٶ�
int max_speed_diff = 700;       // ����ٶȲ�
int turning_counter;            // ת�������
int left = 0;                   // �����ٶ�(δ�޷�)
int right = 0;                  // �����ٶ�(δ�޷�)
int leftCompare = 0;            // �����ٶ�(�޷�)
int rightCompare = 0;           // �����ٶ�(�޷�)
uint32_t current_time = 0;      // ��ǰʱ��
uint32_t point_time = 0;        // �¼�ʱ��
bool in_roundabout = false;     // �Ƿ��ڻ���
bool start_counting = false;    // �Ƿ�ʼ��ʱ

float angle;
extern float sum_left, sum_right, sum_middle;
extern float left_round, right_round;

//�޷���0~10000
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
    // �������Ҵ��������ܺ͵���PWM
    float speed_diff = (sum_left - sum_right) * max_speed_diff;
    // printf("%f,",(sum_left - sum_right));
    // printf("%f,", speed_diff);
    int convert_diff = (int)speed_diff;
//    printf("%d\n",(int)speed_diff);
//    display("sp_dif:%.d\n",convert_diff);
    float sumd = left_round + right_round + sum_middle;
    // system_delay_ms(100);
//    display("%.2f\n",sumd);
//    printf("%.2f\n",sumd);
    // printf("sum_left:%f,",sum_left);
    // printf("sum_right:%f,",sum_right);
    // printf("sum_middle:%f\n",sum_middle);
    if (sum_middle > 0.8 && fabs(sum_left - sum_right) < 0.2) {
        pwm_set_duty(PWMA, base_speed);
        pwm_set_duty(PWMB, base_speed);
//        printf("straight!\n");
    // display("straight!\n");
    }else if (0.85 < fabs(sum_left - sum_right) && fabs(sum_left - sum_right) < 1.3) { //Сת��

        left = (base_speed + speed_diff);
        right = (base_speed - speed_diff);
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
//        printf("turning!\n");
    // display("turning!\n");
    }
//     else if (fabs(sum_left - sum_right) > 1.3){ //��ת��
//         if (turning_counter < 10){
//         turning_counter ++;
//         }else{
//         left = (base_speed + speed_diff) * 1;
//         right = (base_speed - speed_diff)* 1;//������û��Ǳ���ˣ����ȶ������ǵ���Ȩ�صȲ�����
//         limit(left, right);
//         pwm_set_duty(PWMA, leftCompare);
//         pwm_set_duty(PWMB, rightCompare);
// //        printf("turning_big!\n");
//         turning_counter = 0;
//         }
//     } 

    if (!in_roundabout && left_round + right_round + sum_middle > 1.8) {
        // ��������������⵽���ߣ����뻷��
        encoder_clear_count(TIM2_ENCODER);
        encoder_clear_count(TIM3_ENCODER);
//        angle = theta_measure();
        in_roundabout = true;
        start_counting = false; // ���ü�ʱ��־

        display("in_roundabout!\n");

        if (left_round > right_round) {
            left = base_speed;
            right = base_speed - 1000;
        } else {
            left = base_speed - 1000;
            right = base_speed;
        }
    }

    if (in_roundabout) {
        angle = theta_measure(); // ���½Ƕ�
        display("%.2f\n",angle);
        printf("%.2f\n",angle);
        if (angle > 6.15 && !start_counting) { // ��Ϊ��Լת��2pi����ʼ��ʱ
            point_time = system_getval_ms(); // ��¼��ǰʱ��
            start_counting = true; // ��ʼ��ʱ
        }

        if (start_counting) {
            if (current_time - point_time < 3000) { // �������״̬3s
                pwm_set_duty(PWMA, base_speed);
                pwm_set_duty(PWMB, base_speed);
            } else {
                theta_reset();
                angle = 0;
                in_roundabout = false; // �˳�����
                start_counting = false; // ���ü�ʱ��־
                display("out_roundabout!\n");
            }
        }
    }

//  ͣ�������&ʮ��·�ڼ�⣬�����¼����ʱ��������0.6s��ͣ��
    if (sum_middle > 0.9 && sum_left > 0.9 && sum_right > 0.9) { 
        current_time = system_getval_ms();
        if (current_time - point_time > 600) {
            pwm_set_duty(PWMA, 0);
            pwm_set_duty(PWMB, 0);
        }else{
            pwm_set_duty(PWMA, base_speed);
            pwm_set_duty(PWMB, base_speed);
            display("crossing!\n");
        }
    }
    // ���ܷɱ���
    if (sum_middle <0.15 && fabs(sum_left - sum_right) < 0.25) {
        // ���д���������⵽���ף�ֹͣС��
        pwm_set_duty(PWMA, 0);
        pwm_set_duty(PWMB, 0);
//        printf("stopped!\n");
    // display("stopped!\n");
        return;
    }
}
