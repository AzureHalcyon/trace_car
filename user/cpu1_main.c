/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu1_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
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
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中

// **************************** 代码区域 ****************************

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

int time = 0;
int lastTime = 0;
extern int rDuty , lDuty;
extern int BaseSpeed;
extern int StopCount;
extern int L1, L2, M, R1, R2;
void protect();
void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等




    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

//        protect();
        // rRoundDetect();


        // 此处编写需要循环执行的代码
    }
}


void protect(){
    if (normalized_sensors[0] > 0.8 && normalized_sensors[1] > 0.8 && normalized_sensors[2] > 0.8 && normalized_sensors[3] > 0.8 && normalized_sensors[4] > 0.8)
        {
            { // 全黑
                StopCount++;
            }
            if (StopCount > 4)
            { // 停车
                lDuty = 0;
                rDuty = 0;
            }
        }
        if (normalized_sensors[0] < 0.2 && normalized_sensors[1] < 0.2 && normalized_sensors[2] < 0.2 && normalized_sensors[3] < 0.2 && normalized_sensors[4] < 0.2)
        { // 全白
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
// **************************** 代码区域 ****************************
