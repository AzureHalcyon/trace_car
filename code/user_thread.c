#include "user_thread.h"
#include "sensor.h"
#include "control_logic.h"

float error = 0;

void user_init(){
    motor_driver_init();
    read_sensor_init();
}

void user_activities(){
    read_sensor();
    error = calculateError(sensors);
    currentState = determineState(sensors);
    handleState(currentState, sensors);
}