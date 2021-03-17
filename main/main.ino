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
enum class State { Sleep, Wait, Parse, Moving};


// ---------- Constants ----------
// --- Motors ---
const uint8_t ID_MOTOR_1 = 1;
const uint8_t ID_MOTOR_2 = 2;
const uint16_t MODEL_NB_MOTOR_1 = 0;
const uint16_t MODEL_NB_MOTOR_2 = 0;
const uint8_t VERRIN_PIN = 22; //51; // GPIO 4
const uint8_t SOLENOID_PIN = 23; //53; // GPIO 6
const char* NAME_MOTOR_1 = "";
const char* NAME_MOTOR_2 = "";

// --- Messages ---
const int BAUDRATE = 115200;


// ---------- Variables ----------
// --- Motors ---
DynamixelWorkbench dyna_workbench;

// --- Data ---
State current_state = State::Sleep;
String msg = String();
Nut current_nut;



// ---------- Function declarations ----------
// --- Motors ---
void init_motor(uint8_t motor_ID, const char* motor_name, uint16_t model_number);
void stop_motors();
void run_demo();

// --- Messages ---
void check_for_start();
void check_for_stop();
void parse_msg();


// ---------- Function definitions ----------
// --- Motors ---
void init_motor(uint8_t motor_ID, const char* motor_name, uint16_t model_number)
{
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

void run_demo()
{
    float x = 0.096; //current_nut.coord.X / 1000.0f;
    float y = 0.462; //current_nut.coord.Y / 1000.0f;
    send_data("I received: " + msg + ", which converts to: " + nut_to_string(current_nut));

    // send the coordinates to the inverse_kinematics functions to get angles
    float current_angles[4] = {80.0, 80.0, -130.0, 150.0};
    inverse_kinematics(x, y, current_angles);
    send_data("OpenCR: Angles : " + String((int)current_angles[0]) + ", " + String((int)current_angles[1]));

    // move the motors to the wanted angles
    dyna_workbench.goalPosition(ID_MOTOR_1, (int32_t)2095);//current_angles[0]);
    dyna_workbench.goalPosition(ID_MOTOR_2, (int32_t)2095);//current_angles[1]);
    send_data("OpenCR: Moved motors.");

    current_state = State::Wait;
}


void pick()
{
	send_data("I received: " + msg + ", which converts to: " + nut_to_string(current_nut));
	digitalWrite(VERRIN_PIN, HIGH);
	digitalWrite(SOLENOID_PIN, LOW);
	delay(10000);
	digitalWrite(VERRIN_PIN, LOW);
	digitalWrite(SOLENOID_PIN, HIGH);
	dyna_workbench.goalPosition(ID_MOTOR_1, (int32_t)2095);//current_angles[0]);
	dyna_workbench.goalPosition(ID_MOTOR_2, (int32_t)2095);//current_angles[1]);
	delay(10000);
	digitalWrite(VERRIN_PIN, HIGH);
	digitalWrite(SOLENOID_PIN, LOW);
	dyna_workbench.goalPosition(ID_MOTOR_1, (int32_t)1000);
	dyna_workbench.goalPosition(ID_MOTOR_2, (int32_t)1000);
	delay(10000);
	digitalWrite(VERRIN_PIN, LOW);
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
    if (nut.coord.X != 9999 && nut.coord.Y != 9999 && nut.type != 9)
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
    Serial.begin(BAUDRATE);
    init_motor(ID_MOTOR_1, NAME_MOTOR_1, MODEL_NB_MOTOR_1);
    init_motor(ID_MOTOR_2, NAME_MOTOR_2, MODEL_NB_MOTOR_2);
    pinMode(VERRIN_PIN, OUTPUT); // pin 4
    pinMode(SOLENOID_PIN, OUTPUT); // pin 6
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
            //run_demo();
            pick();
            break;
            current_state = State::Wait;
    }
    delay(10);
}
