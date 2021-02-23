#include <Arduino.h>
#include "comm_functions.h"
#include "inverse_kinematics.h"
#include "dynamixel_motors.h"

int baudrate = 115200;
bool run = false;
DynamixelWorkbench motor_1;
DynamixelWorkbench motor_2;

void setup()
{
    Serial.begin(baudrate);
    init_motor(motor_1, 1, "", 0);
    init_motor(motor_2, 2, "", 0);
}

void loop()
{
    String msg = get_data();
    msg = check_for_start_stop_signal(msg, &run);

    if (run)
    {
        // This is the main loop for the program.

        Nut received_nut = parse_nut(msg);
        send_data("I received: " + msg + ", which converts to: " + nut_to_string(received_nut));
        // send the coordinates to the inverse_kinematics functions to get angles
        // move the motors to the wanted angles
    }
    else
    {
        send_data("Waiting for the START command.");
        delay(1000);
    }
}
