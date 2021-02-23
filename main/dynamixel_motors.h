#include <Arduino.h>
#include <DynamixelWorkbench.h>

#ifndef dynamixel_motors_h
#define dynamixel_motors_h

void init_motor(DynamixelWorkbench& motor, uint8_t motor_ID, const char* motor_name, uint16_t model_number);
void move_motor(DynamixelWorkbench& motor, uint8_t id, float desired_angle);

#endif
