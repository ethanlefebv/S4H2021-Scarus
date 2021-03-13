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
enum class Mode { Sleep, Wait, Parse, Moving};


// ---------- Constants ----------
// --- Motors ---
const uint8_t ID_MOTOR_1 = 1;
const uint8_t ID_MOTOR_2 = 2;
const uint16_t MODEL_NB_MOTOR_1 = 0;
const uint16_t MODEL_NB_MOTOR_2 = 0;
const char* NAME_MOTOR_1 = "";
const char* NAME_MOTOR_2 = "";

// --- Messages ---
const int BAUDRATE = 115200;


// ---------- Variables ----------
// --- Motors ---
DynamixelWorkbench motor_1;
DynamixelWorkbench motor_2;

// --- Data ---
Mode current_mode = Mode::Sleep;
String msg = String();
Nut current_nut;



// ---------- Function declarations ----------
// --- Motors ---
void init_motor(DynamixelWorkbench* motor, uint8_t motor_ID, const char* motor_name, uint16_t model_number);
void run_demo();

// --- Messages ---
void check_for_start();
void check_for_stop();
void parse_msg();


// ---------- Function definitions ----------
// --- Motors ---
void init_motor(DynamixelWorkbench* motor, uint8_t motor_ID, const char* motor_name, uint16_t model_number)
{
    const char* error_message;
    motor->init(motor_name, 57600, &error_message);
    motor->ping(motor_ID, &model_number, &error_message);
    motor->jointMode(motor_ID, 0, 0, &error_message);
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
    motor_1.goalPosition(ID_MOTOR_1, (int32_t)2095);//current_angles[0]);
    motor_2.goalPosition(ID_MOTOR_2, (int32_t)2095);//current_angles[1]);
    send_data("OpenCR: Moved motors.");

    current_mode = Mode::Wait;
}

// --- Messages ---
void check_for_start()
{
    msg = get_data();
    if (msg == "START")
    {
        send_data("Starting the program.");
        current_mode = Mode::Wait;
    }
}

void check_for_stop()
{
    if (msg == "STOP")
    {
        send_data("Stopping the program.");
        current_mode = Mode::Sleep;
    }
}

void parse_msg()
{
    Nut nut = parse_nut(msg);
    if (nut.coord.X != 9999 && nut.coord.Y != 9999 && nut.type != 9)
    {
        current_nut = nut;
        current_mode = Mode::Moving;
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
    init_motor(&motor_1, ID_MOTOR_1, NAME_MOTOR_1, MODEL_NB_MOTOR_1);
    init_motor(&motor_2, ID_MOTOR_2, NAME_MOTOR_2, MODEL_NB_MOTOR_2);
}

void loop()
{
    switch (current_mode)
    {
        case Mode::Sleep:
            // Waiting for the signal to start the program
            send_data("Waiting for the START command.");
            delay(1000);
            check_for_start();
            break;

        case Mode::Wait:
            // Waiting for data on the serial port
            msg = get_data();
            if (msg.length() != 0)
            {
                current_mode = Mode::Parse;
            }
            break;

        case Mode::Parse:
            parse_msg();
            break;

        case Mode::Moving:
            run_demo();
            break;
    }
    delay(10);
}
