#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <vector>

class DynamixelWorkbench;
struct Nut;

int32_t degrees_to_int(const float angle);

void move_to_pos(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4]);

bool move_to_pos_wait(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4]);

void init_motor(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motorAngles[4]);

void go_to_home(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motorAngles[4]);

void start_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs);

void stop_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs);

void linear_high(const uint8_t LINEAR_PIN);

void solenoid_high(const uint8_t SOLENOID_PIN);

void pick(const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

void drop(const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

void go_to_pick(const Nut& current_nut, DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

void go_to_drop(const Nut& current_nut, DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

#endif;
