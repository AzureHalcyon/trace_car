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
        return 1; // ȫ��
    }
    if (normalized_sensors[0] > 0.8 && normalized_sensors[1] > 0.8 && normalized_sensors[2] > 0.8 && normalized_sensors[3] > 0.8 && normalized_sensors[4] > 0.8)
    {
        return 2; // ȫ��
    }
    return 0; // �к���
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
        rRoundState = 1; // ���뻷��
                         //        printf("rRoundState: %d\n", rRoundState);
    }
    else if (rRoundCount > 40)
    {
        rRoundState = 2; // �뿪����
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
        lRoundState = 1; // ���뻷��
    }
    else if (lRoundCount > 40)
    {
        lRoundState = 2; // �뿪����
    }
    if (liner == 1)
    {
        lRoundState = 0;
    }
}

void JudgeThread()
{

    BinerySensors();

//��������ʼ��ʱ������//
    if (time_flag==0) {
        time_flag=1;
        system_start();
    }
    car_time=system_getval_ms();

//������˫�߼�⡤����//
    if (((normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]) > 1.6)
          &&(normalized_sensors[0]<0.2||normalized_sensors[1]<0.2||normalized_sensors[2]<0.2||normalized_sensors[3]<0.2||normalized_sensors[0]<0.2)) {
    if ((car_time>5000)&&(car_time<18000)) {                                                  // ��һ����������
      first_flag++;                                                                           //���µ�һ���ڵ��ʱ��
      if (first_flag==1) {
          pre_time=car_time;
      }
      time_interval=system_getval_ms()-pre_time;                                              //����ÿ���ڵ����һ���ڵ��ʱ����
      if ((time_interval>300)&&(time_interval<4000)) {                                        //�ڶ����ڵ���ת
          round_case=2;
          round_count=5;
      }
      else if ((time_interval>4000)&&(time_interval<8000)) {                                 //���ĸ��ڵ���ת
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

//��������������ʱ��⡤����//
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

//��������ȫ�������������//
    if ((normalized_sensors[0]+normalized_sensors[1]+normalized_sensors[2]+normalized_sensors[3]+normalized_sensors[4]) > 4)
    {
        cross_flag=1;                               //ȫ�ڱ�־
        cross_count=30;                            //��־��50���жϺ��0
//        round_direct=1;
//        t_round=0;
//        round_flag=0;
//        flag_count = 0;
//        round_time = 0;
    }

    if (cross_count>0) {                          //ȫ�ڱ�־�ӳ�����
        cross_count--;
    }
    else {
        cross_flag=0;
    }

//��������ȫ�������������//
    if (normalized_sensors[0] < 0.2 && normalized_sensors[1] < 0.2 && normalized_sensors[2] < 0.2 && normalized_sensors[3] < 0.2 && normalized_sensors[4] < 0.2)
    {
        if (cross_flag==1) {                     //����ով�����ȫ�ڣ�һ����ȫ�׾�ͣ��
            lDuty = 0;
            rDuty = 0;
            turn_left_count=0;
            turn_right_count=0;
        }
        else if (turn_left_count>0) {           //����ת�䱣��������50���ж�
            turn_left_count--;
            lDuty=0;
            rDuty=4500;
        }
        else if (turn_right_count>0) {
            turn_right_count--;
            lDuty=4500;
            rDuty=0;
        }                                        //����ȫ��ͣ��
        else {
            lDuty=0;
            rDuty=0;
            turn_left_count=0;
            turn_right_count=0;
        }
    }

//������������ָ���ӳ١�������//
    else if ((round_case!=0)&&(round_count!=0)){
     switch (round_case) {
         case 3:{                                 //��ת
             rDuty=3500;
             lDuty=0;
             //turn_right_count = 40;
             break;
         }
         case 2:{
             rDuty=0;                             //��ת
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

///������������ѭ����������//
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
