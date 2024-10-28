#include "judge.h"

int base_speed = 3000;          // 基础速度
int max_speed_diff = 700;       // 最大速度差
int turning_counter;            // 转弯计数器
int left = 0;                   // 左轮速度(未限幅)
int right = 0;                  // 右轮速度(未限幅)
int leftCompare = 0;            // 左轮速度(限幅)
int rightCompare = 0;           // 右轮速度(限幅)
uint32_t current_time = 0;      // 当前时间
uint32_t point_time = 0;        // 事件时间
bool in_roundabout = false;     // 是否在环岛
bool start_counting = false;    // 是否开始计时

float angle;
extern float sum_left, sum_right, sum_middle;
extern float left_round, right_round;

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
    }else if (0.85 < fabs(sum_left - sum_right) && fabs(sum_left - sum_right) < 1.3) { //小转弯

        left = (base_speed + speed_diff);
        right = (base_speed - speed_diff);
        limit(left, right);
        pwm_set_duty(PWMA, leftCompare);
        pwm_set_duty(PWMB, rightCompare);
//        printf("turning!\n");
    // display("turning!\n");
    }
//     else if (fabs(sum_left - sum_right) > 1.3){ //大转弯
//         if (turning_counter < 10){
//         turning_counter ++;
//         }else{
//         left = (base_speed + speed_diff) * 1;
//         right = (base_speed - speed_diff)* 1;//这里最好还是别加了，不稳定，还是调整权重等参数吧
//         limit(left, right);
//         pwm_set_duty(PWMA, leftCompare);
//         pwm_set_duty(PWMB, rightCompare);
// //        printf("turning_big!\n");
//         turning_counter = 0;
//         }
//     } 

    if (!in_roundabout && left_round + right_round + sum_middle > 1.8) {
        // 三个传感器都检测到黑线，进入环岛
        encoder_clear_count(TIM2_ENCODER);
        encoder_clear_count(TIM3_ENCODER);
//        angle = theta_measure();
        in_roundabout = true;
        start_counting = false; // 重置计时标志

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
        angle = theta_measure(); // 更新角度
        display("%.2f\n",angle);
        printf("%.2f\n",angle);
        if (angle > 6.15 && !start_counting) { // 认为大约转了2pi，开始计时
            point_time = system_getval_ms(); // 记录当前时间
            start_counting = true; // 开始计时
        }

        if (start_counting) {
            if (current_time - point_time < 3000) { // 保持这个状态3s
                pwm_set_duty(PWMA, base_speed);
                pwm_set_duty(PWMB, base_speed);
            } else {
                theta_reset();
                angle = 0;
                in_roundabout = false; // 退出环岛
                start_counting = false; // 重置计时标志
                display("out_roundabout!\n");
            }
        }
    }

//  停车区检测&十字路口检测，如果记录到的时间间隔大于0.6s，停车
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
    // 防跑飞保护
    if (sum_middle <0.15 && fabs(sum_left - sum_right) < 0.25) {
        // 所有传感器都检测到纯白，停止小车
        pwm_set_duty(PWMA, 0);
        pwm_set_duty(PWMB, 0);
//        printf("stopped!\n");
    // display("stopped!\n");
        return;
    }
}
