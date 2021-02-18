#include <Arduino.h>

class Coord
{
public:
    Coord(float x = 0.0, float y = 0.0)
    {
        _x = x;
        _y = y;
    }
    ~Coord() {}

    float get_X()
    {
        return _x;
    }
    float get_Y()
    {
        return _y;
    }

    String print()
    {
        return "X:" + String(_x) + " | Y:" + String(_y);
    }

private:
    float _x;
    float _y;
};

int baudrate = 115200;
bool run = false;
const char ID = '1';
String DESC = "This program parses coordinates and sends back the read coordinate.";

void setup()
{
    Serial.begin(baudrate);
}

void loop()
{
    String msg = get_data();
    msg = check_for_start_stop_signal(msg);

    if (run)
    {
        // This is the main loop for the program.

        //Coord received_coord = parse_coord(msg);
        String rec = parse_coord(msg);
        send_data("I received: " + rec);
        //send_data("I received: " + received_coord.print());
        //send_data("I received: " + msg);
    }
    else
    {
        send_data("Waiting for the START command.");
        delay(1000);
    }
}

/// Verify if the current message says to start
/// or stop the program.
/// Return the message.
String check_for_start_stop_signal(String msg)
{
    if (msg == "START")
    {
        send_data("Starting the program. " + DESC);
        msg = get_data();
        run = true;
    }
    else if (msg == "STOP")
    {
        send_data("Stopping the program.");
        run = false;
    }
    return msg;
}

/// Read data from the serial port.
/// Ignore the message if it was sent by this device.
/// Return the received data.
String get_data()
{
    String tmp = Serial.readStringUntil('\n');
    String data = "";
    // A string of one char would mean the message only contains
    // the ID of the sender, so ignore the message
    if (tmp.length() > 1)
    {
        // The first char is the ID of the sender: if the ID is this
        // device's ID, ignore the message
        if (tmp[0] != ID)
        {
            data = tmp.substring(1);
        }
    }
    return data;
}

/// Parse a coordinate from a String.
/// Return the coordinate if it was valid, else
/// return (9999,9999).
//Coord parse_coord(String data)
String parse_coord(String data)
{
    //Coord coord;
    String coord;
    int separator_index = data.indexOf('|');
    if (separator_index != -1)
    {
        String x = data.substring(0, separator_index);
        String y = data.substring(separator_index + 1);
        //coord = Coord(x.toFloat(), y.toFloat());
        coord = String(x.toFloat()) + '|' + String(y.toFloat());
    }
    else
    {
        coord = "9999.0|9999.0";
        //coord = Coord(9999.0, 9999.0);
    }
    return coord;
}

/// Write data to the serial port.
/// Prepend the ID of the sender to the message.
/// Return the sent message.
String send_data(String data)
{
    String message = ID + data;
    Serial.println(message);
    return message;
}
