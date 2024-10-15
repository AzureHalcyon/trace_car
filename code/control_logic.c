#include "control_logic.h"
#include "private_defines.h"

int round_counter = 0;

CarState currentState = STATE_STRAIGHT;
CarState lastState;
CarState roundFlag;


CarState determineState(int *sensors) {
    // 十字路口/停车区判断：所有传感器为黑色
    int i; 
    if (sensors[0] == 1 && sensors[1] == 1 && sensors[2] == 1 &&
        sensors[3] == 1 && sensors[4] == 1) {     
        if (i!=5){
            i++;
        }else if (i == 5){
            i++;
            return STATE_STRAIGHT;
        }else if (i ==10){
            return STATE_STOP;
        }
    }else{
        i = 0;
    }
    
    // 环岛判断：M白色，L1和R1黑色，同时L2或R2有黑色
    if (sensors[2] == 0 && sensors[1] == 1 && sensors[3] == 1 &&
        (sensors[0] == 1 || sensors[4] == 1)) {
        lastState == STATE_ROUNDABOUT;
        return STATE_ROUNDABOUT;
    }

    // 左转
    if ((sensors[0] == 1 || sensors[1] == 1) && sensors[3] == 0 && sensors[4] == 0) {
        lastState == STATE_TURN_LEFT;
        return STATE_TURN_LEFT;
    }

    // 右转
    if (sensors[3] == 1 && sensors[4] == 1 && (sensors[0] == 0 || sensors[1] == 0)) {
        lastState == STATE_TURN_RIGHT;
        return STATE_TURN_RIGHT;
    }

    // 默认直行
    lastState == STATE_STRAIGHT;
    return STATE_STRAIGHT;
}

//小车可以近似看作一辆双轮差速控制的车，两侧车轮的速度差决定转弯半径。
//假设无打滑、低速行驶，可以忽略惯性影响。我们直接通过差速来控制方向，并且将电机的编码器反馈作为实际速度进行闭环控制，最后考虑乘以一个补偿系数来调整速度。
//差速转向模型：
//转向半径 R与两侧车轮速度的关系：
//左轮速度VL = V - wL * d / 2 , 右轮速度VR = V + wR * d / 2 ,车轮间距d = 150mm , 转向半径R = d / 2 * (VL + VR) / (VR - VL)

void handleState(CarState state, int *sensors) {
    switch (state) {
        case STATE_STRAIGHT: {
            float error = calculateError(sensors);
            float correction = calculatePID(error);

            float left_speed = BASE_SPEED - correction;  // 左轮减去修正量
            float right_speed = BASE_SPEED + correction; // 右轮加上修正量

            setMotorPWM(left_speed, right_speed);
            break;
        }

        case STATE_ROUNDABOUT: {
            float left_speed , right_speed;
            if (round_counter < 1){
                round_counter++;
                if (lastState == STATE_TURN_LEFT){
                    left_speed = BASE_SPEED + BASE_ERROR;
                    right_speed = BASE_SPEED - BASE_ERROR;
                    roundFlag = STATE_TURN_LEFT;
                }else if (lastState == STATE_TURN_RIGHT){
                    left_speed = BASE_SPEED - BASE_ERROR;
                    right_speed = BASE_SPEED + BASE_ERROR;
                    roundFlag = STATE_TURN_RIGHT;
                }
            }else{
                round_counter = 0;
                if (roundFlag == STATE_TURN_LEFT){
                    left_speed = BASE_SPEED - BASE_ERROR;
                    right_speed = BASE_SPEED + BASE_ERROR;
                }else if (roundFlag == STATE_TURN_RIGHT){
                    left_speed = BASE_SPEED + BASE_ERROR;
                    right_speed = BASE_SPEED - BASE_ERROR;
                }
            // 环岛状态下，需根据黑线位置保持小车沿着环岛边缘行驶

            // 增加向环岛转向的倾向
            setMotorPWM(left_speed, right_speed);

            break;
        }

        case STATE_TURN_LEFT||STATE_TURN_RIGHT: {
            float error = calculateError(sensors);
            float correction = calculatePID(error);

            float left_speed = BASE_SPEED + correction;
            float right_speed = BASE_SPEED - correction;
            setMotorPWM(left_speed, right_speed);
            break;
        }

        case STATE_STOP: {
            float left_speed = 0;
            float right_speed = 0;

            setMotorPWM(left_speed, right_speed);
            break;
        }
    }
}
