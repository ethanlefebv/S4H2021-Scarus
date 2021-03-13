/*
Project:     SCARUS
Description: Main code that moves the motors. To be run from the OpenCR.
Authors: Alec Gagnon,      gaga2120
         Étienne Lefebvre, lefe1001
         Robin Mailhot,    mair1803
*/

// ---------- Libraries ----------
#include <Arduino.h>
#include <DynamixelWorkbench.h>
#include "comm_functions.h"
#include "inverse_kinematics.h"

// ---------- Enumerations ----------


// ---------- Constants ----------
const uint8_t ID_MOTOR_1 = 1;
const uint8_t ID_MOTOR_2 = 2;
const uint16_t MODEL_NB_MOTOR_1 = 0;
const uint16_t MODEL_NB_MOTOR_2 = 0;
const char* NAME_MOTOR_1 = "";
const char* NAME_MOTOR_2 = "";

const int BAUDRATE = 115200;

// ---------- Variables ----------
bool run = false;
DynamixelWorkbench motor_1;
DynamixelWorkbench motor_2;

// ---------- Function declarations ----------
//void init_motor(DynamixelWorkbench* motor, uint8_t motor_ID, const char* motor_name, uint16_t model_number);

// ---------- Function definitions ----------
void init_motor(DynamixelWorkbench* motor, uint8_t motor_ID, const char* motor_name, uint16_t model_number)
{
    const char* error_message;
    motor->init(motor_name, 57600, &error_message);
    motor->ping(motor_ID, &model_number, &error_message);
    motor->jointMode(motor_ID, 0, 0, &error_message);
}

// ---------- Main functions ----------
void setup()
{
    Serial.begin(BAUDRATE);
    init_motor(&motor_1, ID_MOTOR_1, NAME_MOTOR_1, MODEL_NB_MOTOR_1);
    init_motor(&motor_2, ID_MOTOR_2, NAME_MOTOR_2, MODEL_NB_MOTOR_2);
}

void loop()
{
    String msg = get_data();
    msg = check_for_start_stop_signal(msg, &run);

    if (run)
    {
        // This is the main loop for the program.

        Nut received_nut = parse_nut(msg);
        float x = 0.096; //received_nut.coord.X / 1000.0f;
        float y = 0.462; //received_nut.coord.Y / 1000.0f;
        send_data("I received: " + msg + ", which converts to: " + nut_to_string(received_nut));

        // send the coordinates to the inverse_kinematics functions to get angles
        float current_angles[4] = {80.0, 80.0, -130.0, 150.0};
        inverse_kinematics(x, y, current_angles);
        send_data("OpenCR: Angles : " + String((int)current_angles[0]) + ", " + String((int)current_angles[1]));

        // move the motors to the wanted angles
        motor_1.goalPosition(ID_MOTOR_1, (int32_t)2095);//current_angles[0]);
        motor_2.goalPosition(ID_MOTOR_2, (int32_t)2095);//current_angles[1]);
        send_data("OpenCR: Moved motors.");
    }
    else
    {
        send_data("Waiting for the START command.");
        delay(1000);
    }
}
