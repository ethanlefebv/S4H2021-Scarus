#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <vector>

class DynamixelWorkbench;

int32_t degrees_to_int(const float angle);

void move_to_pos(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4]);

bool move_to_pos_wait(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4]);

void init_motor(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motorAngles[4]);

void go_to_home(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motorAngles[4]);

void start_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs);

void stop_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs);

void run_test(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs);

void pick(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, const uint8_t linear_solenoid_pin, const uint8_t solenoid_pin);

#endif;
