#include "beep.h"

void beep(){
    gpio_set_level(BEEP, 1);
    system_delay_ms(100);
    gpio_set_level(BEEP, 0);
    system_delay_ms(100);
}

void set_buzzer_frequency(uint32_t freq) {
    pwm_init(ATOM2_CH0_P33_10, freq, freq / 2);
    pwm_set_duty(ATOM2_CH0_P33_10, freq / 2);
}

uint32_t music[3][205] = {
    {9, 10, 9, 6, 9, 0, 6, 10, 9, 10, 12, 10, 9, 10, 9, 6, 8, 0, 10, 9, 12, 10},
    {5, 9, 8, 3, 0, 8, 9, 10, 9, 8, 9, 5, 9, 8, 3, 0, 8, 9, 10, 9, 8, 9, 5, 9, 8, 3, 0, 8, 9, 10, 9, 8, 9, 5, 9, 8, 3, 0, 8, 10, 12, 13, 12, 0, 8, 9, 9, 8, 9, 10, 0, 10, 12, 13, 14, 13, 12, 10, 9, 9, 8, 9, 9, 9, 8, 0, 8, 10, 12, 13, 12, 0, 8, 9, 9, 8, 10, 0, 9, 10, 12, 13, 15, 16, 17, 16, 15, 12, 13, 15, 17, 17, 17, 16, 0, 12, 13, 15, 17, 17, 17, 16, 0}
};

uint32_t lst[3][205] = {
    {1, 1, 1, 1, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 2, 2, 1, 1, 2},
    {2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 6, 6, 3, 1, 2, 2, 2, 2, 4, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 1, 1, 1, 6, 6, 3, 1, 2, 2, 2, 2, 4, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 4, 1, 1, 2, 2, 2, 2, 2, 8}
};

uint32_t tones[] = {21000000, 262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988, 1046, 1174, 1318, 1396, 1568, 1760, 1976};
uint32_t lenth[] = {22, 103};

void play_music(uint32_t song) {
    uint32_t frequency = 21000000;
    uint32_t nw = 0, time_base = 125;
        while (1) {
        set_buzzer_frequency(frequency / tones[music[song][nw]]);
        system_delay_ms(time_base * lst[song][nw] - time_base * 1 / 4);
        set_buzzer_frequency(frequency / tones[0]);
        system_delay_ms(time_base * 1 / 4);
        nw = (nw + 1) % lenth[song];
    }
}
