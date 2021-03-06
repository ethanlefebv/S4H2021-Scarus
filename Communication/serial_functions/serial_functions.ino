#include <Arduino.h>

struct Coord
{
    int X;
    int Y;
};

struct Nut
{
    Coord coord;
    int type;
};

int baudrate = 115200;
bool run = false;
const char ID = '1';
String DESC = "This program parses nuts and coordinates and sends back the read data.";

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

        Nut received_nut = parse_nut(msg);
        send_data("I received: " + msg + ", which converts to: " + nut_to_string(received_nut));
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
Coord parse_coord(String data)
{
    Coord coord;
    int separator_index = data.indexOf('|');
    if (separator_index != -1)
    {
        String x = data.substring(0, separator_index);
        String y = data.substring(separator_index + 1);
        coord.X = x.toInt();
        coord.Y = y.toInt();
    }
    else
    {
        coord.X = 9999;
        coord.Y = 9999;
    }
    return coord;
}

/// Parse and return a Nut from a String.
Nut parse_nut(String data)
{
    Nut nut;
    int separator_index = data.indexOf('/');
    if (separator_index != -1)
    {
        String type = data.substring(0, separator_index);
        String coord_string = data.substring(separator_index + 1);
        nut.type = type.toInt();
        nut.coord = parse_coord(coord_string);
    }
    else
    {
        nut.type = 9;
        nut.coord = parse_coord("");
    }
    return nut;
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

String coord_to_string(Coord coord)
{
    return "X:" + String(coord.X) + ", Y:" + String(coord.Y);
}

String nut_to_string(Nut nut)
{
    return "Type:" + String(nut.type) + ", " + coord_to_string(nut.coord);
}
