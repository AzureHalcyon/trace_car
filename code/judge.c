#include "judge.h"

int base_speed = 3000; // �����ٶ�
int max_speed_diff = 700; // ����ٶȲ�
int turning_counter;
int left = 0;
int right = 0;
int leftCompare = 0;
int rightCompare = 0;

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
    int speed_diff = (sum_left - sum_right) * max_speed_diff;
//    printf("%d\n", speed_diff);

    if (sum_middle > 0.8 && fabs(sum_left - sum_right) < 0.2) {
        pwm_set_duty(PWMA, base_speed);
        pwm_set_duty(PWMB, base_speed);
//        printf("straight!\n");
    }else if (0.85 < fabs(sum_left - sum_right) && fabs(sum_left - sum_right) < 1.3) { //Сת��

        left = (base_speed + speed_diff);
        right = (base_speed - speed_diff);
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
//        printf("turning!\n");
    }else if (fabs(sum_left - sum_right) > 1.3){ //��ת��
        if (turning_counter < 10){
        turning_counter ++;
        }else{
        left = (base_speed + speed_diff) * 1;
        right = (base_speed - speed_diff)* 1;//������û��Ǳ���ˣ����ȶ������ǵ���Ȩ�صȲ�����
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
//        printf("turning_big!\n");
        turning_counter = 0;
        }
    } 

    // ���ܷɱ���
    if (sum_middle <0.15 && fabs(sum_left - sum_right) < 0.25) {
        // ���д���������⵽���ף�ֹͣС��
        pwm_set_duty(PWMA, 0);
        pwm_set_duty(PWMB, 0);
//        printf("stopped!\n");
        return;
    }
}
