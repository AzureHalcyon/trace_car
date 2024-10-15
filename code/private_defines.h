#ifndef __PRIVATE_DEFINES_H
#define __PRIVATE_DEFINES_H

#include "control_logic.h"
#include "motor_driver.h"
#include "steering_pid.h"
#include "sensor.h"

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

#define BASE_SPEED              100     // 基础速度（单位：PWM）
#define BASE_ERROR              30      // 手动设定转向
#define MAX_PWM                 9999     // 最大PWM输出
#define MIN_PWM                 0       // 最小PWM输出
#define SRATIO                  1.5     //pwm最终的比例系数（需要修改）

#endif // !__PRIVATE_DEFINES_H

