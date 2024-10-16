#include "steering_pid.h"

float Kp = 1.0, Ki = 0.0, Kd = 0.5; // PID系数
float error = 0, last_error = 0, integral = 0;

//建立一个简化的力学模型来只对 x 方向进行闭环控制，y 方向通过预设的状态机给出速度。
//通过 PID 计算的误差调整两侧车轮的转速差，从而控制转弯的半径和角度。

/**
 * @brief 定义一个偏差e , 表示小车相对于黑线的偏移量。根据传感器的位置，可以通过加权法计算这个偏差
 * 
 * @param sensors 传感器数组
 * @return float 返回加权后的偏差
 * @note 黑线在左侧偏负，在右侧偏正，e=2×L2+1×L1-1×R1-2×R2
 */
float calculateError(int *sensors) {
    //黑线在左侧偏负，在右侧偏正
    return (2 * sensors[0] + sensors[1] - sensors[3] - 2 * sensors[4]);
}

/**
 * @brief 使用位置式 PID 控制器,根据当前误差来调整车轮的速度差异，以保持小车在黑线的中间
 * 
 * @param error 偏差
 * @return float 返回修正量
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
