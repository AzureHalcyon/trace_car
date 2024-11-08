#include "judge.h"
#include "pid.h"


float DutyCtrl = 0;
int rDuty = 0;
int lDuty = 0;
int cross_flag=0;
int cross_count=0;
int BaseSpeed = 0;

int round_case=0;
int round_count=0;
extern int round_flag=0;
int round_time=0;
int round_direct=0;
int flag_count=0;
int t_round=0;
int round_index=1.6;
int pre_time=0;
int turn_cross_count=0;
int first_round_right_count=0;
int first_round_left_count=0;
extern int time_interval=0;
extern int first_flag=0;
extern int second_flag=0;
extern int time_flag=0;
extern int32 car_time=0;



void JudgeThread()
{
//···开始计时···//
    if ((time_flag==0)&&(menu.car_start==1)) {                                                     //在电机开启的同时计时
        time_flag=1;
        system_start();
    }
    car_time=system_getval_ms();

//·················环境识别部分·················//
//···双线检测···//
    if (((normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]) > 1.6)
          &&(normalized_sensors[0]<0.2||normalized_sensors[1]<0.2||normalized_sensors[2]<0.2||normalized_sensors[3]<0.2||normalized_sensors[0]<0.2)) {
    if ((car_time>0)&&(car_time<18000)) {                                                  // 第一个环岛区间
      first_flag++;                                                                           //记下第一个节点的时间
      if (first_flag==1) {
          pre_time=car_time;
      }
      time_interval=system_getval_ms()-pre_time;                                              //计算每个节点与第一个节点的时间间隔
      if ((time_interval>200)&&(time_interval<4000)) {                                        //第二个节点右转
          if (first_round_left_count>50) {
          round_case=1;                                                                       //右环岛右转（外侧）
          round_count=10;
          first_round_left_count=0;
          }
          else {
              first_round_left_count++;
          }
      }
      else if ((time_interval>4000)&&(time_interval<8000)) {                                 //第四个节点左转

          if (first_round_right_count>100) {
              round_case=2;                                                                      //右环岛左转（内侧）
              round_count=10;
              first_round_right_count=0;
          }
          else {
              first_round_right_count++;
          }
      }
    }
      else if (car_time>18000) {
      second_flag++;
      if (second_flag==1) {
          pre_time=car_time;
      }
      time_interval=system_getval_ms()-pre_time;
      if ((time_interval>250)&&(time_interval<4000)) {
          round_case=3;                                                                      //左环岛左转（外侧）
          round_count=10;
      }
      else if ((time_interval>6000)&&(time_interval<12000)) {
          round_case=4;                                                                      //左环岛右转（内侧）
          round_count=10;
      }
    }
  }

//···出弯保护识别···//
    if ((normalized_sensors[0]+normalized_sensors[1])-(normalized_sensors[3]+normalized_sensors[4])>1.2) {
        turn_left_count=150;
        turn_right_count=0;
    }
    else if ((normalized_sensors[3]+normalized_sensors[4])-(normalized_sensors[0]+normalized_sensors[1])>1.2) {
        turn_right_count=150;
        turn_left_count=0;
    }

//····全黑情况····//
    if ((normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]) > 3.6)
    {
        if ((car_time>12000) && (car_time<35000)&&(menu.emergence_turn_switch==1)){
            round_count=140;
            round_case=5;
        }
        cross_flag=1;                               //全黑标志
        cross_count=20;                             //标志在10次中断后归0
    }

    if (cross_count>0) {                            //全黑标志延迟清零
        cross_count--;
    }
    else {
        cross_flag=0;
    }

//·················电机控制部分·················//
//···转弯指令延迟···//
     if ((round_case!=0)&&(round_count!=0)){
     switch (round_case) {
         case 1:{                                   //进入第一次环岛（右转）
             rDuty=-500;
             lDuty=4000;
             //turn_right_count = 40;
             break;
         }
         case 2:{                                   //离开第一次环岛（左转）
             rDuty=2000;
             lDuty=-3000;
             //turn_left_count = 40;
             break;
         }
         case 3:{                                   //进入第二次环岛（左转）
             rDuty=4000;
             lDuty=500;
             break;
         }
         case 4:{                                   //离开第一次环岛（右转）
             rDuty=-5000;
             lDuty=2000;
             break;
         }
         case 5:{                                   //十字转弯（右转）
//             rDuty=-6200;
//             lDuty=500;
             rDuty=-2500;
             lDuty=0;
             break;
         }
     }
     round_count--;                                 //指令执行相应次数中断的时间
     if (round_count==0) {
         round_case=0;
     }
    }

//···全白情况···//
    else if (normalized_sensors[0] < 0.15 && normalized_sensors[1] < 0.15 && normalized_sensors[2] < 0.15 && normalized_sensors[3] < 0.15 && normalized_sensors[4] < 0.15)
    {
        if (cross_flag==1) {                     //如果刚刚经历过全黑，一遇到全白就停车
            lDuty = 0;
            rDuty = 0;
            turn_left_count=0;
            turn_right_count=0;
        }
        else if (turn_left_count>0) {           //左右转弯保护，持续50次中断
            turn_left_count--;
            lDuty=2000;
            rDuty=4000;
        }
        else if (turn_right_count>0) {
            turn_right_count--;
            lDuty=4000;
            rDuty=2000;
        }                                        //否则全白停车
        else {
            lDuty=0;
            rDuty=0;
//            turn_left_count=0;
//            turn_right_count=0;
        }
    }

//····环岛指令延迟····//

///····正常循迹····//
    else {
        if ((cross_flag==1)&&(car_time>35000)&&(menu.final_accelerate==1)){
            BaseSpeed = 10000;
        }
        else{
            BaseSpeed = 4800;
        }
        DutyCtrl = LinerPID();
        rDuty = BaseSpeed + DutyCtrl;
        lDuty = BaseSpeed - DutyCtrl;
    }

   set_motor_speed(PWMA, rDuty);
   set_motor_speed(PWMB, lDuty);

//    tft180_show_int(0, 100,first_flag , 8);
//    tft180_show_int(0, 120,time_interval , 8);
//    tft180_show_int(40, 100,second_flag , 8);
//    tft180_show_int(0, 140,car_time , 8);
////    tft180_show_float(0,60,normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4],2,3);


}
