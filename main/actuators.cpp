#include "actuators.hpp"
#include "inverse_kinematics.hpp"
#include "nut.hpp"
#include <Arduino.h>
#include <DynamixelWorkbench.h>
#include <vector>

/// Custom function to convert an angle to a value that can be sent
/// to the Dynamixel motors.
int32_t degrees_to_int(const float angle)
{
    return (int32_t)(4095*(angle+180)/360);
}

/// Moves the motors to a wanted position.
void move_to_pos(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4])
{
    for (size_t i = 0; i < motor_IDs.size(); ++i)
    {
        motor.goalPosition(motor_IDs[i], degrees_to_int(angles[i]));
    }
}

/// Calls move_to_pos and waits until the movements are complete.
bool move_to_pos_wait(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4])
{
    bool move_complete = false;
  
    for (int i = 0; i < 100 && !move_complete; ++i)
    {
        move_to_pos(motor, motor_IDs, angles);
         
        int32_t pos0 = 0;
        int32_t pos1 = 0;

        motor.getPresentPositionData(motor_IDs[0], &pos0);
        motor.getPresentPositionData(motor_IDs[1], &pos1);

        move_complete = abs(degrees_to_int(angles[0]) - pos0) < 10 && abs(degrees_to_int(angles[1]) - pos1) < 10;
        
        delay(10);
    }
}

void init_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN)
{
    for (size_t i = 0; i < motor_IDs.size(); ++i)
    {
        const char* motor_name = i+"";
        uint16_t model_number = 0;
        const char* error_message;
        motor.init(motor_name, 57600, &error_message);
        motor.ping(motor_IDs[i], &model_number, &error_message);
        motor.jointMode(motor_IDs[i], 150, 0, &error_message);
        motor.torqueOn(motor_IDs[i]);
    }
}

void go_to_home(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN)
{
    for (int i = 0; i < sizeof(HOMEANGLES)/sizeof(HOMEANGLES[0]); ++i)
    {
        motor_angles[i] = HOMEANGLES[i];
    }
    linear_high(LINEAR_PIN);
    delay(1000);
    move_to_pos_wait(motor, motor_IDs, motor_angles);
    analogWrite(LINEAR_PIN, 0);
}

void start_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs)
{
    for (size_t i = 0; i < motor_IDs.size(); ++i)
    {
        motor.torqueOn(motor_IDs[i]);
    }
}

void stop_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs)
{
    for (size_t i = 0; i < motor_IDs.size(); ++i)
    {
        motor.torqueOff(motor_IDs[i]);
    }
}

void linear_high(const uint8_t LINEAR_PIN)
{
    analogWrite(LINEAR_PIN, 170);
}

void solenoid_high(const uint8_t SOLENOID_PIN)
{
    analogWrite(SOLENOID_PIN, 255);//analogWrite(SOLENOID_PIN, 150);
}

void pick(const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN)
{
    analogWrite(LINEAR_PIN, 0);
    solenoid_high(SOLENOID_PIN);
    delay(500);
    linear_high(LINEAR_PIN);
}

void drop(const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN)
{
    analogWrite(LINEAR_PIN, 0);
    analogWrite(SOLENOID_PIN, 0);
}

void go_to_pick(const Nut& current_nut, DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN)
{
    linear_high(LINEAR_PIN);
    delay(500);
    inverse_kinematics(current_nut.coord.x, current_nut.coord.y, motor_angles);
    move_to_pos_wait(motor, motor_IDs, motor_angles);
    delay(500);
    pick(LINEAR_PIN, SOLENOID_PIN);
    delay(500);
}

void go_to_drop(const Nut& current_nut, DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4], const uint8_t LINEAR_PIN, const uint8_t SOLENOID_PIN)
{
    float dropX = 0.097;
    float dropY = 0.150;
    
    if (current_nut.type == 0)
    {
        dropX = -0.085;
        dropY = 0.250;
    }
    else if (current_nut.type == 1)
    {
        dropX = 0.280;
        dropY = 0.250;
    }

    inverse_kinematics(dropX, dropY, motor_angles);
    move_to_pos_wait(motor, motor_IDs, motor_angles);
    delay(100);
    drop(LINEAR_PIN, SOLENOID_PIN);
}
