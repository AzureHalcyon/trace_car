#ifndef __BEEP_H__
#define __BEEP_H__

#include "zf_common_headfile.h"
#include "math.h"
#include "defines.h"

void beep();
void set_buzzer_frequency(uint32_t freq);
void play_music(uint32_t song);

#endif // __BEEP_H__