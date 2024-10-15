#ifndef __CONTROL_LOGIC_H
#define __CONTROL_LOGIC_H

// #include "private_defines.h"
#include "zf_common_headfile.h"

typedef enum {
    STATE_STRAIGHT,   // ֱ��
    STATE_TURN_LEFT,  // ��ת
    STATE_TURN_RIGHT, // ��ת
    STATE_ROUNDABOUT, // ����
    STATE_STOP,       // ͣ��
    STATE_BLIND       // �ܷ�
} CarState;


#endif // !__CONTROL_LOGIC_H

