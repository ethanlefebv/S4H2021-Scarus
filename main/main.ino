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
int32_t degrees_to_int(const float angle)
{
    return (int32_t)(4095*(angle+180)/360);
}

bool wait_for_motion(float angle0, float angle1)
{
  for(int i = 0; i < 100; ++i)
  {
    int32_t pos1;
    int32_t pos2;
    dyna_workbench.getPresentPositionData(ID_MOTOR_1, &pos1);
    dyna_workbench.getPresentPositionData(ID_MOTOR_2, &pos2);
    Serial.println("-----");
    Serial.println(degrees_to_int(angle0));
    Serial.println(pos1);
    Serial.println(degrees_to_int(angle1));
    Serial.println(pos2);
    if(abs(degrees_to_int(angle0)-pos1) < 3 && abs(degrees_to_int(angle1)-pos2) < 3)
    {
      break;
    }
    delay(10);
  }
}

void init_motor(uint8_t motor_ID)
{
    const char* motor_name = "";
    uint16_t model_number = 0;
    const char* error_message;
    dyna_workbench.init(motor_name, 57600, &error_message);
    dyna_workbench.ping(motor_ID, &model_number, &error_message);
    dyna_workbench.jointMode(motor_ID, 0, 0, &error_message);
    dyna_workbench.torqueOn(motor_ID);
    dyna_workbench.goalPosition(motor_ID, (int32_t)2047);
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

    // move the motors to the desired angles
    dyna_workbench.goalPosition(ID_MOTOR_1, (int32_t)2095);//current_angles[0]);
    dyna_workbench.goalPosition(ID_MOTOR_2, (int32_t)2095);//current_angles[1]);
    send_data("OpenCR: Moved motors.");

    current_state = State::Wait;
}

void run_test()
{
  start_motors();
  
  float angles[4] = {10, -10, -27, 27};
  dyna_workbench.goalPosition(ID_MOTOR_1, degrees_to_int(angles[0]));
  dyna_workbench.goalPosition(ID_MOTOR_2, degrees_to_int(angles[1]));
  wait_for_motion(angles[0], angles[1]);
  
  inverse_kinematics(0.105f, 0.46f, angles);
  Serial.println("-----");
  Serial.println(degrees_to_int(angles[0]));
  Serial.println(angles[0]);
  Serial.println(degrees_to_int(angles[1]));
  Serial.println(angles[1]);
  dyna_workbench.goalPosition(ID_MOTOR_1, degrees_to_int(angles[0]));
  dyna_workbench.goalPosition(ID_MOTOR_2, degrees_to_int(angles[1]));
  wait_for_motion(angles[0], angles[1]);

  inverse_kinematics(0.105f, 0.20f, angles);
  Serial.println("-----");
  Serial.println(degrees_to_int(angles[0]));
  Serial.println(angles[0]);
  Serial.println(degrees_to_int(angles[1]));
  Serial.println(angles[1]);
  dyna_workbench.goalPosition(ID_MOTOR_1, degrees_to_int(angles[0]));
  dyna_workbench.goalPosition(ID_MOTOR_2, degrees_to_int(angles[1]));
  wait_for_motion(angles[0], angles[1]);

  inverse_kinematics(0.40f, 0.20f, angles);
  Serial.println("-----");
  Serial.println(degrees_to_int(angles[0]));
  Serial.println(angles[0]);
  Serial.println(degrees_to_int(angles[1]));
  Serial.println(angles[1]);
  dyna_workbench.goalPosition(ID_MOTOR_1, degrees_to_int(angles[0]));
  dyna_workbench.goalPosition(ID_MOTOR_2, degrees_to_int(angles[1]));
  wait_for_motion(angles[0], angles[1]);

  inverse_kinematics(0.105f, 0.20f, angles);
  Serial.println("-----");
  Serial.println(degrees_to_int(angles[0]));
  Serial.println(angles[0]);
  Serial.println(degrees_to_int(angles[1]));
  Serial.println(angles[1]);
  dyna_workbench.goalPosition(ID_MOTOR_1, degrees_to_int(angles[0]));
  dyna_workbench.goalPosition(ID_MOTOR_2, degrees_to_int(angles[1]));
  wait_for_motion(angles[0], angles[1]);

  inverse_kinematics(-0.20f, 0.20f, angles);
  Serial.println("-----");
  Serial.println(degrees_to_int(angles[0]));
  Serial.println(angles[0]);
  Serial.println(degrees_to_int(angles[1]));
  Serial.println(angles[1]);
  dyna_workbench.goalPosition(ID_MOTOR_1, degrees_to_int(angles[0]));
  dyna_workbench.goalPosition(ID_MOTOR_2, degrees_to_int(angles[1]));
  wait_for_motion(angles[0], angles[1]);

  stop_motors();
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
    const int BAUDRATE = 115200;
    
    Serial.begin(BAUDRATE);
  
    init_motor(ID_MOTOR_1);
    init_motor(ID_MOTOR_2);
    pinMode(LINEAR_SOLENOID_PIN, OUTPUT); // pin 4
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
            run_test();
            //pick();
            delay(1000);
            //send_data("Done");
            current_state = State::Wait;
            break;
    }
    delay(10);
}
