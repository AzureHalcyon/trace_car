#ifndef __DEFINES_H__
#define __DEFINES_H__

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

#define BEEP                    (P33_10)

#define WHEEL_DISTANCE          0.12  //小车左右轮之间的距离，单位：米
#define PULSES_PER_REV          271 //每转编码器脉冲数(待测)
#define WHEEL_RADIUS            0.0225 //车轮半径，单位：米
#define PULSE_DISTANCE          (2 * 3.1415926 * WHEEL_RADIUS / PULSES_PER_REV)//每个脉冲对应的距离(待测)

#endif // !__DEFINES_H__
