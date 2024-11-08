/*
 * car_switch_show_menu.c
 *
 *  Created on: 2024Äê11ÔÂ8ÈÕ
 *      Author: lenovo
 */

#include "car_switch_menu.h"
#include "car_switch_key.h"
#include "defines.h"

menu_struct menu={0};

void car_switch_init(void){
    menu.high_number=1;
    menu.car_start=0;
    menu.emergence_turn_switch=1;
    menu.final_accelerate=1;
}

void car_switch_show_menu(void){
     switch (menu.high_number){
        case 1:{
            tft180_clear();
            tft180_show_string(0,0,"switch<--");
            tft180_show_string(0,20,"turn");
            tft180_show_string(0,40,"acc");
            tft180_show_int(100,0,menu.car_start,1);
            tft180_show_int(100,20,menu.emergence_turn_switch,1);
            tft180_show_int(100,40,menu.final_accelerate,1);
            tft180_show_string(40,100,"good luck!");
            break;
        }
        case 2:{
            tft180_clear();
            tft180_show_string(0,0,"switch");
            tft180_show_string(0,20,"turn<--");
            tft180_show_string(0,40,"acc");
            tft180_show_int(100,0,menu.car_start,1);
            tft180_show_int(100,20,menu.emergence_turn_switch,1);
            tft180_show_int(100,40,menu.final_accelerate,1);
            tft180_show_string(40,100,"good luck!");
            break;
        }
        case 3:{
            tft180_clear();
            tft180_show_string(0,0,"switch");
            tft180_show_string(0,20,"turn");
            tft180_show_string(0,40,"acc<--");
            tft180_show_int(100,0,menu.car_start,1);
            tft180_show_int(100,20,menu.emergence_turn_switch,1);
            tft180_show_int(100,40,menu.final_accelerate,1);
            tft180_show_string(40,100,"good luck!");
            break;
        }
      }
}

void car_switch_refresh_menu(user_operation_enum keyt){
     switch (keyt) {
         case 0:{
             if (menu.high_number>1) {
                 menu.high_number--;
             }
             else {
                 menu.high_number=3;
             }
             break;
         }
         case 1:{
             if (menu.high_number<3) {
                 menu.high_number++;
             }
             else {
                 menu.high_number=1;
             }
             break;
         }
         case 2:{
             switch (menu.high_number) {
                 case 1:{
                     if (menu.car_start==1) {
                         gpio_set_level(STBY,0);
                         menu.car_start=0;
                     }
                     break;
                 }
                 case 2:{
                     if (menu.emergence_turn_switch==1) {
                         menu.emergence_turn_switch=0;
                     }
                     break;
                 }
                 case 3:{
                     if (menu.final_accelerate==1) {
                         menu.final_accelerate=0;
                     }
                     break;
                 }
             }
             break;
         }
         case 3:{
             switch (menu.high_number) {
                 case 1:{
                     if (menu.car_start==0) {
                         menu.car_start=1;
                         gpio_set_level(STBY,1);
                     }
                     break;
                 }
                 case 2:{
                     if (menu.emergence_turn_switch==0) {
                         menu.emergence_turn_switch=1;
                     }
                     break;
                 }
                 case 3:{
                     if (menu.final_accelerate==0) {
                         menu.final_accelerate=1;
                     }
                     break;
                 }
             }
             break;
         }
         case 4:{
             break;
         }
     }
}



