#include <Arduino.h>
#include "comm_functions.h"
#include "inverse_kinematics.h"

int baudrate = 115200;
bool run = false;

void setup()
{
    Serial.begin(baudrate);
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
    }
    else
    {
        send_data("Waiting for the START command.");
        delay(1000);
    }
}
