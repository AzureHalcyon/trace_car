#ifndef __CONTROL_LOGIC_H
#define __CONTROL_LOGIC_H

// #include "private_defines.h"
#include "zf_common_headfile.h"

typedef enum {
    STATE_STRAIGHT,   // 直行
    STATE_TURN_LEFT,  // 左转
    STATE_TURN_RIGHT, // 右转
    STATE_ROUNDABOUT, // 环岛
    STATE_STOP,       // 停车
    STATE_BLIND       // 跑飞
} CarState;

extern CarState currentState;//当前的状态

CarState determineState(int *sensors);
void handleState(CarState state, int *sensors);

#endif // !__CONTROL_LOGIC_H

