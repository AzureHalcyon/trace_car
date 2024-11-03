#include "judge.h"

int liner = 0;
int rRoundCount = 0;
int lRoundCount = 0;
int rRoundState = 0;
int lRoundState = 0;
int StopCount = 0;
float rRoundCtrl = 0;
float lRoundCtrl = 0;
float DutyCtrl = 0;
int rDuty = 0;
int lDuty = 0;
int BaseSpeed = 4500;

extern int L1, L2, M, R1, R2;
extern float sum_left, sum_middle, sum_right;

int LinerDetect()
{
    if (normalized_sensors[0] < 0.2 && normalized_sensors[1] < 0.2 && normalized_sensors[2] < 0.2 && normalized_sensors[3] < 0.2 && normalized_sensors[4] < 0.2)
    {
        return 1; // 全白
    }
    if (normalized_sensors[0] > 0.8 && normalized_sensors[1] > 0.8 && normalized_sensors[2] > 0.8 && normalized_sensors[3] > 0.8 && normalized_sensors[4] > 0.8)
    {
        return 2; // 全黑
    }
    return 0; // 有黑线
}

void rRoundDetect()
{
    liner = L1 + L2 + M + R1 + R2;
//       printf("liner: %d\n", liner);
    if (liner == 2 && R1 == 1)
    {
        rRoundCount++;
        //        printf("rRoundCount: %d\n", rRoundCount);
    }
    if (liner == 2 && R2 == 1)
    {
        rRoundCount++;
        //        printf("rRoundCount: %d\n", rRoundCount);
    }
    if (rRoundCount > 25 && rRoundCount < 40)
    {
        rRoundState = 1; // 进入环岛
                         //        printf("rRoundState: %d\n", rRoundState);
    }
    else if (rRoundCount > 40)
    {
        rRoundState = 2; // 离开环岛
                         //        printf("rRoundState: %d\n", rRoundState);
    }
    if (liner == 1)
    {
        rRoundState = 0;
        //        printf("rRoundState: %d\n", rRoundState);
    }
}

void lRoundDetect()
{
    liner = L1 + L2 + M + R1 + R2;
    //    printf("liner: %d\n", liner);
    if (liner == 2 && L1 == 1)
    {
        lRoundCount++;
    }
    if (liner == 2 && L2 == 1)
    {
        lRoundCount++;
    }
    if (lRoundCount > 25 && lRoundCount < 40)
    {
        lRoundState = 1; // 进入环岛
    }
    else if (lRoundCount > 40)
    {
        lRoundState = 2; // 离开环岛
    }
    if (liner == 1)
    {
        lRoundState = 0;
    }
}

void JudgeThread()
{
     BinerySensors();
    // rRoundDetect();
    // lRoundDetect();

    // if (rRoundState == 1 || lRoundState == 1 || rRoundState == 2 || lRoundState == 2){//如果rRoundState或lRoundState为1或2
    //     if (rRoundState == 2)
    //     { // 离开环岛
    //         rRoundCtrl = RoundPID();
    //         rDuty = BaseSpeed - rRoundCtrl * 0.2;
    //         lDuty = BaseSpeed + rRoundCtrl * 0.2;
    //     }
    //     else if (rRoundState == 1)
    //     { // 进入环岛
    //         rRoundCtrl = RoundPID();
    //         rDuty = BaseSpeed * 1.4 + rRoundCtrl;
    //         lDuty = BaseSpeed * 0.7 - rRoundCtrl;
    //     }

    //     if (lRoundState == 2)
    //     { // 离开环岛
    //         lRoundCtrl = RoundPID();
    //         lDuty = BaseSpeed * 1.1 + lRoundCtrl * 0.6;
    //         rDuty = BaseSpeed * 0.9 - lRoundCtrl * 0.6;
    //     }
    //     else if (lRoundState == 1)
    //     { // 进入环岛
    //         lRoundCtrl = RoundPID();
    //         lDuty = BaseSpeed * 0.5 + lRoundCtrl * 0.4;
    //         rDuty = BaseSpeed * 1.6 - lRoundCtrl * 0.4;
    //     }
    // }else{
    DutyCtrl = LinerPID();
    rDuty = BaseSpeed + DutyCtrl;
    lDuty = BaseSpeed - DutyCtrl;
    if (normalized_sensors[0] > 0.8 && normalized_sensors[1] > 0.8 && normalized_sensors[3] > 0.8 && normalized_sensors[4] > 0.8)
    {
        if(StopCount <= 4)
        { // 全黑
            StopCount++;
        }
        if (StopCount > 4)
        { // 停车
            lDuty = 0;
            rDuty = 0;
        }
    }else{
        StopCount = 0;
    }
    if (normalized_sensors[0] < 0.2 && normalized_sensors[1] < 0.2 && normalized_sensors[2] < 0.2 && normalized_sensors[3] < 0.2 && normalized_sensors[4] < 0.2)
    { // 全白
       if (StopCount < 5){
           StopCount++;
           if (sum_right < sum_left)
           {
               lDuty = 0;
               rDuty = BaseSpeed;
           }
           else
           {
               lDuty = BaseSpeed;
               rDuty = 0;
           }
            lDuty = 0;
            rDuty = 0;
        }
    }
    set_motor_speed(PWMA, rDuty);
    set_motor_speed(PWMB, lDuty);
}
