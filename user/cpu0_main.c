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
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.4
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "math.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设

#define PWMA                    (ATOM0_CH1_P21_3)
#define PWMB                    (ATOM0_CH0_P21_2)
#define AIN1                    (P22_2)
#define AIN2                    (P22_3)
#define BIN1                    (P22_1)
#define BIN2                    (P22_0)
#define STBY                    (P23_1)
#define ENA                     (P33_7)
#define ENB                     (P33_6)

#define Sensor_L2               (ADC0_CH0_A0)
#define Sensor_L1               (ADC0_CH1_A1)
#define Sensor_M                (ADC0_CH2_A2)
#define Sensor_R1               (ADC0_CH3_A3)
#define Sensor_R2               (ADC0_CH4_A4)

// **************************** 代码区域 ****************************

void turns();

int sensors[5];
int threshold_white[5] = {53, 64, 93, 70, 64}; // 每个传感器的纯白阈值
int threshold_black[5] = {19, 21, 34, 20, 22}; // 每个传感器的纯黑阈值
float normalized_sensors[5];
int white_threshold = 0.9; // 纯白阈值

int base_speed = 4000; // 基础速度
int max_speed_diff = 1000; // 最大速度差

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    encoder_quad_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);
    gpio_init(STBY, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(AIN2, GPO, 0, GPO_PUSH_PULL);
    gpio_init(BIN1, GPO, 1, GPO_PUSH_PULL);
    gpio_init(BIN2, GPO, 0, GPO_PUSH_PULL);
    adc_init(Sensor_L1, ADC_8BIT);
    adc_init(Sensor_L2, ADC_8BIT);
    adc_init(Sensor_M, ADC_8BIT);
    adc_init(Sensor_R1, ADC_8BIT);
    adc_init(Sensor_R2, ADC_8BIT);
    pwm_init(PWMA, 1000, 0);
    pwm_init(PWMB, 1000, 0);
    pit_ms_init(CCU60_CH0 , 25);

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码

        // 此处编写需要循环执行的代码
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);
    // encoder_get_count(TIM2_ENCODER);
    turns();

}

void turns(){
    sensors[0] = adc_mean_filter_convert(Sensor_L2, 10);//纯白大约50~53，纯黑大约17~19
    sensors[1] = adc_mean_filter_convert(Sensor_L1, 10);//纯白大约62~66，纯黑大约19~21
    sensors[2] = adc_mean_filter_convert(Sensor_M, 10); //纯白大约89~91，纯黑大约32~34（94~98，27~34）
    sensors[3] = adc_mean_filter_convert(Sensor_R1, 10);//纯白大约61~65，纯黑大约18~20（纯白也有68~70）
    sensors[4] = adc_mean_filter_convert(Sensor_R2, 10);//纯白大约64~68，纯黑大约19~22



//    system_delay_ms(100);
    
//    printf("sensors: ");
//    printf("%d,",sensors[0]);
//    printf("%d,",sensors[1]);
//    printf("%d,",sensors[2]);
//    printf("%d,",sensors[3]);
//    printf("%d\n",sensors[4]);

    //如果两侧的传感器小于纯白，根据与纯白的差值调整PWM,中间如果大于纯黑，同理。
    //思路：每个传感器的阈值有所不同，先进行单位化，然后求和，根据和的大小，调整PWM，采用差速驱动

    // 转弯逻辑

    for (int i = 0; i < 5; i++) {
        normalized_sensors[i] = (float)(sensors[i] - threshold_black[i]) / (threshold_white[i] - threshold_black[i]);
    }

    float sum_left = normalized_sensors[0] + normalized_sensors[1];
    float sum_right = normalized_sensors[3] + normalized_sensors[4];
    float sum_middle = normalized_sensors[2];

//    printf("normalized_sensors: ");
//    printf("%f,",normalized_sensors[0]);
//    printf("%f,",normalized_sensors[1]);
//    printf("%f,",normalized_sensors[2]);
//    printf("%f,",normalized_sensors[3]);
//    printf("%f\n",normalized_sensors[4]);
//
    printf("sum: ");
    printf("%f,",sum_left);
    printf("%f,",sum_middle);
    printf("%f\n",sum_right);

    if (sum_middle < 0.3 && sum_right > 0.7 && sum_left > 0.7) {
        // 中间传感器检测到黑线，直行
        pwm_set_duty(PWMA, base_speed);
        pwm_set_duty(PWMB, base_speed);
        printf("straight!\n");
    } else if (fabs(sum_left - sum_right) > 1.2){
        // 根据左右传感器的总和调整PWM
        int speed_diff = (int)((sum_left - sum_right) * max_speed_diff);
        pwm_set_duty(PWMA, base_speed - speed_diff);
        pwm_set_duty(PWMB, base_speed + speed_diff);
        printf("turning!\n");
    }

    // 防跑飞保护

    if (sum_middle > 0.9 && fabs(sum_left - sum_right) < 0.25) {
        // 所有传感器都检测到纯白，停止小车
        pwm_set_duty(PWMA, 0);
        pwm_set_duty(PWMB, 0);
        printf("stopped!\n");
        return;
    }


}

#pragma section all restore
// **************************** 代码区域 ****************************
