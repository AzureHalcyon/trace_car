#include "encoder.h"

float calculate_turn_radians(int left_encoder_pulses, int right_encoder_pulses) {
    float delta_left = left_encoder_pulses * PULSE_DISTANCE;
    float delta_right = right_encoder_pulses * PULSE_DISTANCE;
    float theta = (delta_right - delta_left) / WHEEL_DISTANCE;
    return theta;
}

float theta_measure() {
    float left_encoder_pulses = encoder_get_count(TIM2_ENCODER);
    float right_encoder_pulses = encoder_get_count(TIM3_ENCODER);
    float theta = calculate_turn_radians(left_encoder_pulses, right_encoder_pulses);
    printf("left:%f,",left_encoder_pulses);
    printf("right:%f,",right_encoder_pulses);
    printf("theta: %f\n", theta);
    return theta;
}

void theta_reset() {
    encoder_clear_count(TIM2_ENCODER);
    encoder_clear_count(TIM3_ENCODER);
//    theta = 0;
}
