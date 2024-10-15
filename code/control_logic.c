#include "control_logic.h"
#include "private_defines.h"

int round_counter = 0;

CarState currentState = STATE_STRAIGHT;
CarState lastState;
CarState roundFlag;


CarState determineState(int *sensors) {
    // ʮ��·��/ͣ�����жϣ����д�����Ϊ��ɫ
    static int i=0;
    if (sensors[0] == 1 && sensors[1] == 1 && sensors[2] == 1 &&
        sensors[3] == 1 && sensors[4] == 1) {     
        if (i>10){
            return STATE_STOP;
        }else {
            i++;
            return STATE_STRAIGHT;
        }
    }//待测试
    
    // �����жϣ�M��ɫ��L1��R1��ɫ��ͬʱL2��R2�к�ɫ
    else{
    i=0;
    if (sensors[2] == 0 && (sensors[1] == 1 || sensor[0] ==1) && (sensors[3] == 1 || sensor[4] ==1 )) {
        lastState == STATE_ROUNDABOUT;
        return STATE_ROUNDABOUT;
    }

    // ��ת
    if ((sensors[0] == 1 || sensors[1] == 1) && sensors[2] == 0 && sensors[3] == 0 && sensors[4] == 0) {
        lastState == STATE_TURN_LEFT;
        return STATE_TURN_LEFT;
    }

    // ��ת
    if ((sensors[3] == 1 || sensors[4] == 1) && sensors[2] == 0 && sensors[0] == 0 && sensors[1] == 0) {
        lastState == STATE_TURN_RIGHT;
        return STATE_TURN_RIGHT;
    }
    }

    // Ĭ��ֱ��
    lastState == STATE_STRAIGHT;
    return STATE_STRAIGHT;
}

//С�����Խ��ƿ���һ��˫�ֲ��ٿ��Ƶĳ������೵�ֵ��ٶȲ����ת��뾶��
//�����޴򻬡�������ʻ�����Ժ��Թ���Ӱ�졣����ֱ��ͨ�����������Ʒ��򣬲��ҽ�����ı�����������Ϊʵ���ٶȽ��бջ����ƣ�����ǳ���һ������ϵ���������ٶȡ�
//����ת��ģ�ͣ�
//ת��뾶 R�����೵���ٶȵĹ�ϵ��
//�����ٶ�VL = V - wL * d / 2 , �����ٶ�VR = V + wR * d / 2 ,���ּ��d = 150mm , ת��뾶R = d / 2 * (VL + VR) / (VR - VL)

void handleState(CarState state, int *sensors) {
    switch (state) {
        case STATE_STRAIGHT: {
            float error = calculateError(sensors);
            float correction = calculatePID(error);

            float left_speed = BASE_SPEED - correction;  // ���ּ�ȥ������
            float right_speed = BASE_SPEED + correction; // ���ּ���������

            setMotorPWM(left_speed, right_speed);
            break;
        }

        case STATE_ROUNDABOUT: {
            float left_speed , right_speed;
            if (round_counter < 1){
                round_counter++;
                if (lastState == STATE_TURN_LEFT){
                    left_speed = BASE_SPEED - BASE_ERROR;
                    right_speed = BASE_SPEED + BASE_ERROR;
                    roundFlag = STATE_TURN_LEFT;
                }else if (lastState == STATE_TURN_RIGHT){
                    left_speed = BASE_SPEED + BASE_ERROR;
                    right_speed = BASE_SPEED - BASE_ERROR;
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
            // ����״̬�£�����ݺ���λ�ñ���С�����Ż�����Ե��ʻ

            // �����򻷵�ת�������
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
