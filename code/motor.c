#include "motor.h"
#include "defines.h"
#include "zf_driver_pwm.h"

void set_motor_speed(pwm_channel_enum pwm_channel, int speed)
{
    uint32 duty = abs(speed);
    limit(&duty);

    if (pwm_channel == PWMA)
    {
        speed_division_A(speed);
        pwm_set_duty(PWMA, duty);
    }
    else if (pwm_channel == PWMB)
    {
        speed_division_B(speed);
        pwm_set_duty(PWMB, duty);
    }
}

void limit(int *value)
{
    if (*value > 10000)
    {
        *value = 10000;
    }
    else if (*value < 0)
    {
        *value = 0;
    }
}

void speed_division_A(int speed)
{
    if (speed > 0)
    {
        gpio_set_level(AIN1, 1);
        gpio_set_level(AIN2, 0);
    }
    else
    {
        gpio_set_level(AIN1, 0);
        gpio_set_level(AIN2, 1);
    }
}

void speed_division_B(int speed)
{
    if (speed > 0)
    {
        gpio_set_level(BIN1, 1);
        gpio_set_level(BIN2, 0);
    }
    else
    {
        gpio_set_level(BIN1, 0);
        gpio_set_level(BIN2, 1);
    }
}
