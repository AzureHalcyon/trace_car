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
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.9.4
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

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

// **************************** �������� ****************************

void turns();

int sensors[5];

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ�������

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

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);
    // encoder_get_count(TIM2_ENCODER);
    turns();

}

void turns(){
    sensors[0] = adc_mean_filter_convert(Sensor_L2, 5);//���״�Լ50~53�����ڴ�Լ17~19
    sensors[1] = adc_mean_filter_convert(Sensor_L1, 5);//���״�Լ62~66�����ڴ�Լ19~21
    sensors[2] = adc_mean_filter_convert(Sensor_M, 5); //���״�Լ89~91�����ڴ�Լ32~34��94~98��27~34��
    sensors[3] = adc_mean_filter_convert(Sensor_R1, 5);//���״�Լ61~65�����ڴ�Լ18~20������Ҳ��68~70��
    sensors[4] = adc_mean_filter_convert(Sensor_R2, 5);//���״�Լ64~68�����ڴ�Լ19~22



    system_delay_ms(100);
    printf("%d,",sensors[0]);
    printf("%d,",sensors[1]);
    printf("%d,",sensors[2]);
    printf("%d,",sensors[3]);
    printf("%d\n",sensors[4]);

}

#pragma section all restore
// **************************** �������� ****************************
