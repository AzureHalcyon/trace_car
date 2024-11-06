#include "judge.h"
#include "pid.h"

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
int cross_flag=0;
int cross_count=0;
int BaseSpeed = 4500;

int round_case=0;
int round_count=0;
int round_flag=0;
int round_time=0;
int round_direct=0;
int flag_count=0;
int t_round=0;
int round_index=1.6;
int pre_time=0;
int time_interval=0;
int first_flag=0;
int second_flag=0;
int time_flag=0;
int car_time=0;

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

//···开始计时···//
    if (time_flag==0) {
        time_flag=1;
        system_start();
    }
    car_time=system_getval_ms();

//···双线检测···//
    if (((normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]) > 1.6)
          &&(normalized_sensors[0]<0.2||normalized_sensors[1]<0.2||normalized_sensors[2]<0.2||normalized_sensors[3]<0.2||normalized_sensors[0]<0.2)) {
    if ((car_time>5000)&&(car_time<18000)) {                                                  // 第一个环岛区间
      first_flag++;                                                                           //记下第一个节点的时间
      if (first_flag==1) {
          pre_time=car_time;
      }
      time_interval=system_getval_ms()-pre_time;                                              //计算每个节点与第一个节点的时间间隔
      if ((time_interval>300)&&(time_interval<4000)) {                                        //第二个节点右转
          round_case=2;
          round_count=5;
      }
      else if ((time_interval>4000)&&(time_interval<8000)) {                                 //第四个节点左转
          round_case=3;
          round_count=5;
      }
    }
      else if (car_time>18000) {
      second_flag++;
      if (second_flag==1) {
          pre_time=car_time;
      }
      time_interval=system_getval_ms()-pre_time;
      if ((time_interval>250)&&(time_interval<4000)) {
          round_case=3;
          round_count=5;
      }
      else if ((time_interval>6000)&&(time_interval<10000)) {
          round_case=2;
          round_count=5;
      }
    }
  }

    if ((normalized_sensors[0]+normalized_sensors[1])-(normalized_sensors[3]+normalized_sensors[4])>1) {
        turn_left_count=60;
        turn_right_count=0;
    }
    else if ((normalized_sensors[3]+normalized_sensors[4])-(normalized_sensors[0]+normalized_sensors[1])>1) {
        turn_right_count=60;
        turn_left_count=0;
    }

//····环岛定时检测···//
//    int time=system_getval();
//    if ((normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]) > 1.6){
//    if ((time>5)&&(time<7)) {
//       round_case=2;
//       round_count=10;
//    }
//    else if ((time>10)&&(time<12)) {
//        round_case=1;
//        round_count=10;
//    }
//    else if ((time>20)&&(time<22)) {
//        round_case=1;
//        round_count=10;
//    }
//    else if ((time>28)&&(time<30)) {
//        round_case=2;
//        round_count=10;
//    }
//    }
//

//····全黑情况····//
    if ((normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]) > 4)
    {
        cross_flag=1;                               //全黑标志
        cross_count=30;                            //标志在50次中断后归0
//        round_direct=1;
//        t_round=0;
//        round_flag=0;
//        flag_count = 0;
//        round_time = 0;
    }

    if (cross_count>0) {                          //全黑标志延迟清零
        cross_count--;
    }
    else {
        cross_flag=0;
    }

//····全白情况····//
    if (normalized_sensors[0] < 0.2 && normalized_sensors[1] < 0.2 && normalized_sensors[2] < 0.2 && normalized_sensors[3] < 0.2 && normalized_sensors[4] < 0.2)
    {
        if (cross_flag==1) {                     //如果刚刚经历过全黑，一遇到全白就停车
            lDuty = 0;
            rDuty = 0;
            turn_left_count=0;
            turn_right_count=0;
        }
        else if (turn_left_count>0) {           //左右转弯保护，持续50次中断
            turn_left_count--;
            lDuty=0;
            rDuty=4500;
        }
        else if (turn_right_count>0) {
            turn_right_count--;
            lDuty=4500;
            rDuty=0;
        }                                        //否则全白停车
        else {
            lDuty=0;
            rDuty=0;
            turn_left_count=0;
            turn_right_count=0;
        }
    }

//····环岛指令延迟····//
    else if ((round_case!=0)&&(round_count!=0)){
     switch (round_case) {
         case 3:{                                 //左转
             rDuty=3500;
             lDuty=0;
             //turn_right_count = 40;
             break;
         }
         case 2:{
             rDuty=0;                             //右转
             lDuty=3500;
             //turn_left_count = 40;
             break;
         }
         case 1:{
             rDuty=3000;
             lDuty=3000;
             break;
         }
     }
     round_count--;
     if (round_count==0) {
         round_case=0;
     }
    }

///····正常循迹····//
    else {

        if ((normalized_sensors[0] < 0.2 && normalized_sensors[4] < 0.2 && normalized_sensors[2] > 0.8))
        {
            BaseSpeed = 5500;
            DutyCtrl = LinerPID();
            rDuty = BaseSpeed + DutyCtrl;
            lDuty = BaseSpeed - DutyCtrl;
        }
        else{
            BaseSpeed = 5500;
            DutyCtrl = LinerPID();
            rDuty = BaseSpeed + DutyCtrl;
            lDuty = BaseSpeed - DutyCtrl;
        }
    }

//    tft180_clear();
      tft180_show_int(0, 100,first_flag , 8);
      tft180_show_int(0, 120,time_interval , 8);
      tft180_show_int(50, 100,second_flag , 8);
      tft180_show_int(0, 140,car_time , 8);
//    tft180_show_int(40, 100, round_flag, 3);
//    tft180_show_int(80,100,round_direct,3);
////    tft180_show_float(0,0,normalized_sensors[0],2,3);
////    tft180_show_float(50,0,normalized_sensors[1],2,3);
////    tft180_show_float(30,20,normalized_sensors[2],2,3);
////    tft180_show_float(0,40,normalized_sensors[3],2,3);
////    tft180_show_float(50,40,normalized_sensors[4],2,3);
////    tft180_show_float(0,60,normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4],2,3);
    set_motor_speed(PWMA, rDuty);
    set_motor_speed(PWMB, lDuty);
//    uint32 tim = system_getval();
//    printf("time:%d\n",tim);
//    printf("%d\n",t_round);


//       if (StopCount < 5){
//           StopCount++;
//           if (sum_right < sum_left)
//           {
//               lDuty = 0;
//               rDuty = BaseSpeed;
//           }
//           else
//           {
//               lDuty = BaseSpeed;
//               rDuty = 0;
//           }
//            lDuty = 0;
//            rDuty = 0;
//        }
}
