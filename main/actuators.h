#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <vector>

class DynamixelWorkbench;
struct Nut;

int32_t degrees_to_int(const float angle);

void move_to_pos(std::vector<DynamixelWorkbench>& motors, const std::vector<uint8_t>& motor_IDs, float angles[4]);

bool move_to_pos_wait(std::vector<DynamixelWorkbench>& motors, const std::vector<uint8_t>& motor_IDs, float angles[4]);

void init_motors(std::vector<DynamixelWorkbench>  motors, const std::vector<uint8_t>& motor_IDs, float motor_angles[4]);

void go_to_home(std::vector<DynamixelWorkbench>& motors, const std::vector<uint8_t>& motor_IDs, float motor_angles[4]);

void start_motors(std::vector<DynamixelWorkbench>& motors, const std::vector<uint8_t>& motor_IDs);

void stop_motors(std::vector<DynamixelWorkbench>& motors, const std::vector<uint8_t>& motor_IDs);

void linear_high(const uint8_t LINEAR_PIN);

void solenoid_high(const uint8_t SOLENOID_PIN);

void pick(const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

void drop(const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

void go_to_pick(const Nut& current_nut, std::vector<DynamixelWorkbench>& motors, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

void go_to_drop(const Nut& current_nut, std::vector<DynamixelWorkbench>& motors, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN);

#endif;
