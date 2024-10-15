#include "steering_pid.h"

// PID 控制代码
float Kp = 1.0, Ki = 0.0, Kd = 0.5; // PID系数
float error = 0, last_error = 0, integral = 0;
float base_speed = 100; // 基础速度

//定义一个偏差e , 表示小车相对于黑线的偏移量。根据传感器的位置，可以通过加权法计算这个偏差e=2×L2+1×L1-1×R1-2×R2
//建立一个简化的力学模型来只对 x 方向进行闭环控制，y 方向通过预设的状态机给出速度。
//通过 PID 计算的误差调整两侧车轮的转速差，从而控制转弯的半径和角度。

float calculateError(int *sensors) {
    //黑线在左侧偏负，在右侧偏正
    return (2 * sensors[0] + sensors[1] - sensors[3] - 2 * sensors[4]);
}

// 输入: 当前偏差error
float calculatePID(float error) {
    // 计算PID控制值
    float P = Kp * error;
    integral += error;
    float I = Ki * integral;
    float D = Kd * (error - last_error);
    last_error = error;
    
    // 总的调整量
    float delta = P + I + D;
    return delta;
    // // 更新左轮和右轮速度
    // float left_speed = base_speed - delta;
    // float right_speed = base_speed + delta;
    
    // setMotorPWM(left_speed, right_speed);
}