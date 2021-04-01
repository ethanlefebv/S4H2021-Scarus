#include "actuators.h"
#include "inverse_kinematics.h"
#include <Arduino.h>
#include <DynamixelWorkbench.h>
#include <vector>

int32_t degrees_to_int(const float angle)
{
    return (int32_t)(4095*(angle+180)/360);
}

void move_to_pos(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4])
{
  for(size_t i = 0; i < motor_IDs.size(); ++i)
  {
      motor.goalPosition(motor_IDs[i], degrees_to_int(angles[i]));
  }
}

bool move_to_pos_wait(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float angles[4])
{
    for(int i = 0; i < 100; ++i)
    {
        move_to_pos(motor, motor_IDs, angles);
         
        std::vector<int32_t> vPos;
        vPos.reserve(motor_IDs.size());
        
        int doneMotors = 0;
        for(size_t j = 0; j < motor_IDs.size(); ++j)
        {
            motor.getPresentPositionData(motor_IDs[j], &vPos[j]);
            
            if(abs(degrees_to_int(angles[j])-vPos[j]) < 3)
            {
                ++doneMotors;
            }
        }
        
        if(doneMotors == motor_IDs.size())
        {
          break;
        }
        delay(10);
    }
}

void init_motor(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4])
{
    for(const auto motor_ID : motor_IDs)
    {
        const char* motor_name = "";
        uint16_t model_number = 0;
        const char* error_message;
        motor.init(motor_name, 57600, &error_message);
        motor.ping(motor_ID, &model_number, &error_message);
        motor.jointMode(motor_ID, 0, 0, &error_message);
        motor.torqueOn(motor_ID);
    }

    go_to_home(motor, motor_IDs, motor_angles);
}

void go_to_home(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, float motor_angles[4])
{
    for(int i = 0; i < sizeof(HOMEANGLES)/sizeof(HOMEANGLES[0]); ++i)
    {
        motor_angles[i] = HOMEANGLES[i];
    }
    move_to_pos_wait(motor, motor_IDs, motor_angles);
}

void start_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs)
{
    for(const auto motor_ID : motor_IDs )
    {
        motor.torqueOn(motor_ID);
    }
}

void stop_motors(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs)
{
    for(const auto motor_ID : motor_IDs )
    {
        motor.torqueOff(motor_ID);
    }
}

void run_test(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs)
{
    start_motors(motor, motor_IDs);
    
    float angles[4] = {10, -10, -27, 27};
    move_to_pos_wait(motor, motor_IDs, angles);
    
    inverse_kinematics(0.105f, 0.46f, angles);
    Serial.println("-----");
    Serial.println(degrees_to_int(angles[0]));
    Serial.println(angles[0]);
    Serial.println(degrees_to_int(angles[1]));
    Serial.println(angles[1]);
    move_to_pos_wait(motor, motor_IDs, angles);
  
    inverse_kinematics(0.105f, 0.20f, angles);
    Serial.println("-----");
    Serial.println(degrees_to_int(angles[0]));
    Serial.println(angles[0]);
    Serial.println(degrees_to_int(angles[1]));
    Serial.println(angles[1]);
    move_to_pos_wait(motor, motor_IDs, angles);
  
    inverse_kinematics(0.40f, 0.20f, angles);
    Serial.println("-----");
    Serial.println(degrees_to_int(angles[0]));
    Serial.println(angles[0]);
    Serial.println(degrees_to_int(angles[1]));
    Serial.println(angles[1]);
    move_to_pos_wait(motor, motor_IDs, angles);
  
    inverse_kinematics(0.105f, 0.20f, angles);
    Serial.println("-----");
    Serial.println(degrees_to_int(angles[0]));
    Serial.println(angles[0]);
    Serial.println(degrees_to_int(angles[1]));
    Serial.println(angles[1]);
    move_to_pos_wait(motor, motor_IDs, angles);
  
    inverse_kinematics(-0.20f, 0.20f, angles);
    Serial.println("-----");
    Serial.println(degrees_to_int(angles[0]));
    Serial.println(angles[0]);
    Serial.println(degrees_to_int(angles[1]));
    Serial.println(angles[1]);
    move_to_pos_wait(motor, motor_IDs, angles);
  
    stop_motors(motor, motor_IDs);
}

void pick(DynamixelWorkbench& motor, const std::vector<uint8_t>& motor_IDs, const uint8_t linear_solenoid_pin, const uint8_t solenoid_pin)
{
    digitalWrite(linear_solenoid_pin, HIGH);
    digitalWrite(solenoid_pin, LOW);
    delay(10000);
    digitalWrite(linear_solenoid_pin, LOW);
    digitalWrite(solenoid_pin, HIGH);
    for(auto motor_ID : motor_IDs)
    {
        motor.goalPosition(motor_ID, (int32_t)2095);//current_angles[1]);
    }
    delay(10000);
    digitalWrite(linear_solenoid_pin, HIGH);
    digitalWrite(solenoid_pin, LOW);
    for(auto motor_ID : motor_IDs)
    {
        motor.goalPosition(motor_ID, (int32_t)2095);//current_angles[1]);
    }
    delay(10000);
    digitalWrite(linear_solenoid_pin, LOW);
    digitalWrite(solenoid_pin, HIGH);
}
