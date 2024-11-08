/*
 * car_switch_key.h
 *
 *  Created on: 2024Äê11ÔÂ8ÈÕ
 *      Author: lenovo
 */

#ifndef CODE_CAR_SWITCH_KEY_H_
#define CODE_CAR_SWITCH_KEY_H_

#include "zf_common_headfile.h"

#define KEY_UP_PIN    (P13_3)
#define KEY_DOWN_PIN    (P11_2)
#define KEY_BACK_PIN    (P20_7)
#define KEY_IN_PIN    (P20_6)

void NEUQ_key_init(void);


typedef enum{
    key_pressed,
    key_released
} key_status_enum;

typedef enum{
    key_up_pressed,
    key_down_pressed,
    key_back_pressed,
    key_in_pressed,
    key_none_pressed
} user_operation_enum;

typedef struct
{
     gpio_pin_enum key_pin;
     key_status_enum key_last;
     key_status_enum key_output;
     int key_t;
}key_struct;

void scan_a_key(key_struct* key);
user_operation_enum NEUQ_key_scan(void);



#endif /* CODE_CAR_SWITCH_KEY_H_ */
