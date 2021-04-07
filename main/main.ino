/*
Project:     SCARUS
Description: Main code that moves the motors. To be run from the OpenCR.
Authors: Alec Gagnon,      gaga2120
         Étienne Lefebvre, lefe1001
         Robin Mailhot,    mair1803
         Charles Caya
*/

// ---------- Libraries ----------
#include <Arduino.h>
#include <DynamixelWorkbench.h>
#include <vector>
#include "comm_functions.h"
#include "actuators.h"
#include "inverse_kinematics.h"
#include "nut.h"


// ---------- Enumerations ----------
enum class State { Sleep, Wait, Parse, Moving };

// ---------- Constants ----------
// --- Motors ---
const std::vector<uint8_t> MOTOR_IDS = { (const uint8_t)1, (const uint8_t)2 };
const uint8_t LINEAR_PIN = 5;
const uint8_t SOLENOID_PIN = 6;

// ---------- Variables ----------
// --- Motors ---
DynamixelWorkbench dyna1;
DynamixelWorkbench dyna2;
std::vector<DynamixelWorkbench*> dynas = { &dyna1, &dyna2 };

// --- Data ---
State current_state = State::Sleep;
String msg = String();
Nut current_nut;
float motor_angles[4];

// ---------- Main functions ----------
void setup()
{
    const int BAUDRATE = 115200;
    Serial.begin(BAUDRATE);

    init_motors(dynas, MOTOR_IDS, motor_angles);
    pinMode(LINEAR_PIN, OUTPUT);
    pinMode(SOLENOID_PIN, OUTPUT);
}

void loop()
{
    switch (current_state)
    {
        case State::Sleep:
        {
            // Waiting for the signal to start the program
            send_data("Waiting for the START command.");
            delay(100);
            msg = get_data();
            if(should_start(msg))
            {
                send_data("Starting the program.");
                start_motors(dynas, MOTOR_IDS);
                go_to_home(dynas, MOTOR_IDS, motor_angles);
                current_state = State::Wait;
            }
            break;
        }

        case State::Wait:
        {
            // Waiting for data on the serial port
            msg = get_data();
            if (msg.length() != 0)
            {
                current_state = State::Parse;
            }
            break;
        }

        case State::Parse:
        {
            // checks for start, stop and sets nut values
            Nut nut = parse_nut(msg);

            if (nut.is_valid)
            {
                current_state = State::Moving;
                current_nut = nut;
            }
            else if (should_stop(msg))
            {
                send_data("Stopping the program.");
                current_state = State::Sleep;
            }         
            break;
        }

        case State::Moving:
        {
            go_to_pick(current_nut, dynas, MOTOR_IDS, motor_angles, LINEAR_PIN, SOLENOID_PIN);
            send_data("Picked");
            go_to_drop(current_nut, dynas, MOTOR_IDS, motor_angles, LINEAR_PIN, SOLENOID_PIN);
            
            send_data("Done");
            current_state = State::Wait;
            break;
        }
    }
    delay(10);
}
