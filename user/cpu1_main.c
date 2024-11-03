/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.20
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "math.h"
#include "inits.h"
#include "defines.h"
#include "sensor.h"
#include "encoder.h"
#include "judge.h"
#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��

// **************************** �������� ****************************

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

int time = 0;
int lastTime = 0;
extern int rDuty , lDuty;
extern int BaseSpeed;
extern int StopCount;
extern int L1, L2, M, R1, R2;
void protect();
void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������




    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���

//        protect();
        // rRoundDetect();


        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}


void protect(){
    if (normalized_sensors[0] > 0.8 && normalized_sensors[1] > 0.8 && normalized_sensors[2] > 0.8 && normalized_sensors[3] > 0.8 && normalized_sensors[4] > 0.8)
        {
            { // ȫ��
                StopCount++;
            }
            if (StopCount > 4)
            { // ͣ��
                lDuty = 0;
                rDuty = 0;
            }
        }
        if (normalized_sensors[0] < 0.2 && normalized_sensors[1] < 0.2 && normalized_sensors[2] < 0.2 && normalized_sensors[3] < 0.2 && normalized_sensors[4] < 0.2)
        { // ȫ��
            if (lastTime < 20)
            {
                lastTime++;
                //            display("restoring!\n");
                if (sum_right < sum_left)
                {
                    lDuty = -BaseSpeed;
                    rDuty = BaseSpeed;
                }
                else
                {
                    lDuty = BaseSpeed;
                    rDuty = -BaseSpeed;
                }
            }
            else
            {
                //        display("stopped!\n");
                lDuty = 0;
                rDuty = 0;
            }
        }
        else
        {
            lastTime = 0;
        }
        //    display("rDuty:%d\n",rDuty);
        //    display("lDuty:%d\n",lDuty);
}
#pragma section all restore
// **************************** �������� ****************************
