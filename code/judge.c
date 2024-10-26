#include "judge.h"

int base_speed = 3000; // 基础速度
int max_speed_diff = 700; // 最大速度差
int turning_counter;
int left = 0;
int right = 0;
int leftCompare = 0;
int rightCompare = 0;

//限幅：0~10000
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
    // 根据左右传感器的总和调整PWM
    int speed_diff = (sum_left - sum_right) * max_speed_diff;
//    printf("%d\n", speed_diff);

    if (sum_middle > 0.8 && fabs(sum_left - sum_right) < 0.2) {
        pwm_set_duty(PWMA, base_speed);
        pwm_set_duty(PWMB, base_speed);
//        printf("straight!\n");
    }else if (0.85 < fabs(sum_left - sum_right) && fabs(sum_left - sum_right) < 1.3) { //小转弯

        left = (base_speed + speed_diff);
        right = (base_speed - speed_diff);
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
//        printf("turning!\n");
    }else if (fabs(sum_left - sum_right) > 1.3){ //大转弯
        if (turning_counter < 10){
        turning_counter ++;
        }else{
        left = (base_speed + speed_diff) * 1;
        right = (base_speed - speed_diff)* 1;//这里最好还是别加了，不稳定，还是调整权重等参数吧
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
//        printf("turning_big!\n");
        turning_counter = 0;
        }
    } 

    // 防跑飞保护
    if (sum_middle <0.15 && fabs(sum_left - sum_right) < 0.25) {
        // 所有传感器都检测到纯白，停止小车
        pwm_set_duty(PWMA, 0);
        pwm_set_duty(PWMB, 0);
//        printf("stopped!\n");
        return;
    }
}
