#include <Arduino.h>

bool run = false;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    String msg = get_message();

    if (msg == "START")
    {
        send_message("Starting the program.");
        msg = get_message();
        run = true;
    }
    else if (msg == "STOP")
    {
        send_message("Stopping the program.");
        run = false;
    }

    if (run)
    {
        if (msg != "")
        {
            send_message("I heard : " + msg);
        }
    }
    else
    {
        send_message("Waiting for the START command.");
        delay(1000);
    }
}

String get_message()
{
    return Serial.readStringUntil("\n");
}

void send_message(String message)
{
    Serial.println(message);
}
