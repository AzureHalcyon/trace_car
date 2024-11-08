/*
 * car_switch_key.c
 *
 *  Created on: 2024Äê11ÔÂ8ÈÕ
 *      Author: lenovo
 */

#include "car_switch_key.h"
#include "zf_common_headfile.h"

key_struct key_up={0};
key_struct key_down={0};
key_struct key_back={0};
key_struct key_in={0};
//static int t=0;

void NEUQ_key_init(void)
{
    gpio_init(KEY_UP_PIN, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY_DOWN_PIN, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY_BACK_PIN, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY_IN_PIN, GPI, 1, GPI_PULL_UP);

    key_up.key_pin=KEY_UP_PIN;
    key_down.key_pin=KEY_DOWN_PIN;
    key_back.key_pin=KEY_BACK_PIN;
    key_in.key_pin=KEY_IN_PIN;
}

void scan_a_key(key_struct* key)
{

    key_status_enum key_now= gpio_get_level(key->key_pin);

    static int t=0;
    if (key_now==key_pressed)
        {
          if (t==0)
          {
             key->key_output=key_pressed;
             t=1;
          }
          else  {  key->key_output=key_released; }
        }
      else key->key_output=key_released;

    if ((key->key_output==key_released)&&(key_now==key_released))
    {   t=0; }
}

user_operation_enum NEUQ_key_scan(void)
{
    scan_a_key(&key_up);
    scan_a_key(&key_down);
    scan_a_key(&key_back);
    scan_a_key(&key_in);
   if(key_up.key_output==key_pressed)
     {
       return 0;
     }
   if(key_down.key_output==key_pressed)
     {
         return 1;
     }
   if(key_back.key_output==key_pressed)
     {
         return 2;
     }
   if(key_in.key_output==key_pressed)
     {
         return 3;
     }
   else return 4;
}


























