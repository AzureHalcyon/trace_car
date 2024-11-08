/*
 * car_switch_menu.h
 *
 *  Created on: 2024Äê11ÔÂ8ÈÕ
 *      Author: lenovo
 */

#include "car_switch_key.h"

#ifndef CODE_CAR_SWITCH_MENU_H_
#define CODE_CAR_SWITCH_MENU_H_

typedef struct{
    int emergence_turn_switch;
    int car_start;
    int final_accelerate;
    int high_number;
}menu_struct;

extern menu_struct menu;

void car_switch_show_menu(void);
void car_switch_refresh_menu(user_operation_enum keyt);


#endif /* CODE_CAR_SWITCH_MENU_H_ */
