#include <Arduino.h>
#include "dynamixel_motors.h"

void init_motor(DynamixelWorkbench& motor, uint8_t motor_ID, const char* motor_name, uint16_t model_number)
{
    const char* error_message;
    motor.init(motor_name, 57600, &error_message);
    motor.ping(motor_ID, &model_number, &error_message);
    motor.jointMode(motor_ID, 0, 0, &error_message);
}

void move_motor(DynamixelWorkbench motor, uint8_t id, float desired_angle)
{
    motor.goalPosition(id, desired_angle);
}
