#include <Arduino.h>

bool run = false;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    String msg = get_data();

    if (msg == "START")
    {
        send_data("Starting the program.");
        msg = get_data();
        run = true;
    }
    else if (msg == "STOP")
    {
        send_data("Stopping the program.");
        run = false;
    }

    if (run)
    {
        if (msg != "")
        {
            send_data("I heard : " + msg);
        }
    }
    else
    {
        send_data("Waiting for the START command.");
        delay(1000);
    }
}

/// Read data from the serial port.
/// Ignore the message if it was sent by this device.
/// Return the received data.
String get_data()
{
    String tmp = Serial.readStringUntil("\n");
    String data = "";
    // A string of one char would mean the message only contains
    // the ID of the sender, so ignore the message
    if (tmp.length() > 1)
    {
        // The first char is the ID of the sender: if the ID is this
        // device's ID, ignore the message
        if (tmp[0] != '1')
        {
            data = tmp.substring(1);
        }
    }
    return data;
}

// Point get_coord()
// {
//     //TODO
// }

/// Write data to the serial port.
/// Prepend the ID of the sender ('1' for the OpenCR) to the message.
/// Return the sent message.
void send_data(String data)
{
    String message = "1" + data;
    Serial.println(message);
    return message;
}
