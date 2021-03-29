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
#include "comm_functions.h"
#include "inverse_kinematics.h"


// ---------- Enumerations ----------
enum class State { Sleep, Wait, Parse, Moving };


// ---------- Constants ----------
// --- Motors ---
const uint8_t ID_MOTOR_1 = 1;
const uint8_t ID_MOTOR_2 = 2;
const uint8_t LINEAR_SOLENOID_PIN = 22; //51; // GPIO 4
const uint8_t SOLENOID_PIN = 23; //53; // GPIO 6


// ---------- Variables ----------
// --- Motors ---
DynamixelWorkbench dyna_workbench;

// --- Data ---
State current_state = State::Sleep;
String msg = String();
Nut current_nut;



// ---------- Function declarations ----------
// --- Motors ---
void init_motor(uint8_t motor_ID);
void stop_motors();
void run_demo();

// --- Messages ---
void check_for_start();
void check_for_stop();
void parse_msg();


// ---------- Function definitions ----------
// --- Motors ---
void init_motor(uint8_t motor_ID)
{
    const char* motor_name = "";
    uint16_t model_number = 0;
    const char* error_message;
    dyna_workbench.init(motor_name, 57600, &error_message);
    dyna_workbench.ping(motor_ID, &model_number, &error_message);
    dyna_workbench.jointMode(motor_ID, 0, 0, &error_message);
    dyna_workbench.torqueOff(motor_ID, &error_message);
}

void start_motors()
{
    dyna_workbench.torqueOn(ID_MOTOR_1);
    dyna_workbench.torqueOn(ID_MOTOR_2);
}

void stop_motors()
{
    dyna_workbench.torqueOff(ID_MOTOR_1);
    dyna_workbench.torqueOff(ID_MOTOR_2);
}

int32_t DegreesToInt(const float angle)
{
    return (int32_t)(4095*angle/360);
}

void run_test()
{
    dyna_workbench.goalPosition(ID_MOTOR_1, (int32_t)2047);
    dyna_workbench.goalPosition(ID_MOTOR_2, (int32_t)2047);

    float angles[4] = {180, 180, 161, 199};
    inverse_kinematics(0.105f, 0.505f, angles);
    delay(1000);
    dyna_workbench.goalPosition(ID_MOTOR_1, DegreesToInt(angles[1]));
    dyna_workbench.goalPosition(ID_MOTOR_2, DegreesToInt(angles[0]));

    inverse_kinematics(0.105f, 0.40f, angles);
    delay(1000);
    dyna_workbench.goalPosition(ID_MOTOR_1, DegreesToInt(angles[1]));
    dyna_workbench.goalPosition(ID_MOTOR_2, DegreesToInt(angles[0]));

    inverse_kinematics(0.20f, 0.40f, angles);
    delay(1000);
    dyna_workbench.goalPosition(ID_MOTOR_1, DegreesToInt(angles[1]));
    dyna_workbench.goalPosition(ID_MOTOR_2, DegreesToInt(angles[0]));

    delay(1000);
}


void pick()
{
	send_data("I received: " + msg + ", which converts to: " + nut_to_string(current_nut));
	digitalWrite(LINEAR_SOLENOID_PIN, HIGH);
	digitalWrite(SOLENOID_PIN, LOW);
	delay(10000);
	digitalWrite(LINEAR_SOLENOID_PIN, LOW);
	digitalWrite(SOLENOID_PIN, HIGH);
	dyna_workbench.goalPosition(ID_MOTOR_1, (int32_t)2095);//current_angles[0]);
	dyna_workbench.goalPosition(ID_MOTOR_2, (int32_t)2095);//current_angles[1]);
	delay(10000);
	digitalWrite(LINEAR_SOLENOID_PIN, HIGH);
	digitalWrite(SOLENOID_PIN, LOW);
	dyna_workbench.goalPosition(ID_MOTOR_1, (int32_t)1000);
	dyna_workbench.goalPosition(ID_MOTOR_2, (int32_t)1000);
	delay(10000);
	digitalWrite(LINEAR_SOLENOID_PIN, LOW);
	digitalWrite(SOLENOID_PIN, HIGH);
}


// --- Messages ---
void check_for_start()
{
    msg = get_data();
    if (msg == "START")
    {
        send_data("Starting the program.");
        start_motors();
        current_state = State::Wait;
    }
}

void check_for_stop()
{
    if (msg == "STOP")
    {
        send_data("Stopping the program.");
        stop_motors();
        current_state = State::Sleep;
    }
}

void parse_msg()
{
    Nut nut = parse_nut(msg);
    if (nut.coord.X != INVALID_COORD && nut.coord.Y != INVALID_COORD && nut.type != INVALID_NUT_TYPE)
    {
        current_nut = nut;
        current_state = State::Moving;
    }
    else
    {
        check_for_stop();
    }
}


// ---------- Main functions ----------
void setup()
{
    const int BAUDRATE = 115200;
    
    Serial.begin(BAUDRATE);
  
    init_motor(ID_MOTOR_1);
    init_motor(ID_MOTOR_2);
    pinMode(LINEAR_SOLENOID_PIN, OUTPUT); // pin 4
    pinMode(SOLENOID_PIN, OUTPUT); // pin 6
    //run_test();
}

void loop()
{
    switch (current_state)
    {
        case State::Sleep:
            // Waiting for the signal to start the program
            send_data("Waiting for the START command.");
            delay(100);
            check_for_start();
            break;

        case State::Wait:
            // Waiting for data on the serial port
            msg = get_data();
            if (msg.length() != 0)
            {
                current_state = State::Parse;
            }
            break;

        case State::Parse:
        	// checks for start, stop and sets nut values
            parse_msg();
            break;

        case State::Moving:
            run_test();
            //pick();
            delay(1000);
            //send_data("Done");
            current_state = State::Wait;
            break;
    }
    delay(10);
}
