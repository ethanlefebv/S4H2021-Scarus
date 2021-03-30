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


// ---------- Enumerations ----------
enum class State { Sleep, Wait, Parse, Moving };


// ---------- Constants ----------
// --- Motors ---
const uint8_t ID_MOTOR_1 = 1;
const uint8_t ID_MOTOR_2 = 2;
const std::vector<uint8_t> MOTOR_IDS = { ID_MOTOR_1, ID_MOTOR_2};
const uint8_t LINEAR_SOLENOID_PIN = 22; //51; // GPIO 4
const uint8_t SOLENOID_PIN = 23; //53; // GPIO 6


// ---------- Variables ----------
// --- Motors ---
DynamixelWorkbench dyna_workbench;

// --- Data ---
State current_state = State::Sleep;
String msg = String();
Nut current_nut;


// ---------- Main functions ----------
void setup()
{
    const int BAUDRATE = 115200;  
    Serial.begin(BAUDRATE);
  
    init_motor(dyna_workbench, MOTOR_IDS);
    pinMode(LINEAR_SOLENOID_PIN, OUTPUT); // pin 4
    pinMode(SOLENOID_PIN, OUTPUT); // pin 6
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
            if(check_for_start(msg))
            {
              start_motors(dyna_workbench, MOTOR_IDS);
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
            Nut nut;
            int parseRes = parse_msg(msg, nut);
            if(parseRes == 1)
            {
                current_state = State::Moving;
                current_nut = nut;
            }
            
            if(parseRes == 0)
            {
              current_state = State::Sleep;
            }
            break;
        }

        case State::Moving:
        {
            run_test(dyna_workbench, MOTOR_IDS);
            //pick();
            delay(1000);
            //send_data("Done");
            current_state = State::Wait;
            break;
        }
    }
    delay(10);
}
