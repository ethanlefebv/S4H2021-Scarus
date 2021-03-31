#include "comm_functions.h"
#include <Arduino.h>

const char ID = '1';

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

/// Parse an int from a String.
/// Return the read value with the param int_data.
String parse_data(const String& data, int& int_data)
{
    String output;
    int front_separator_index = data.indexOf('/');
    String next_sub_string = data.substring(front_separator_index+1);
    int back_separator_index = next_sub_string.indexOf('/')+1;
    
    if (front_separator_index != -1 && back_separator_index != -1)
    {
        int_data = data.substring(front_separator_index+1, back_separator_index).toInt();
        output = data.substring(back_separator_index);
    }
    else
    {
        send_data("Error: Communication");
        int_data = INVALID_INPUT;
        output = "Error";
    }
    return output;
}

/// Parse and return a Nut from a String.
Nut parse_nut(const String& data)
{
    Nut nut;
    int nutX;
    int nutY;
    parse_data(parse_data(parse_data(data, nutX), nutY), nut.type);

    nut.coord.x = (nutX == INVALID_INPUT) ? INVALID_COORD : nutX / 1000.0;
    nut.coord.y = (nutY == INVALID_INPUT) ? INVALID_COORD : nutY / 1000.0;

    if (nut.type == INVALID_INPUT)
    {
        nut.type = INVALID_NUT_TYPE;
    }

    // Slide bed limit dimensions
    if (nut.coord.x > 0.180 && nut.coord.x != INVALID_COORD) { nut.coord.x = 0.180; }
    if (nut.coord.x < 0.015 && nut.coord.x != INVALID_COORD) { nut.coord.x = 0.015; }
    
    if (nut.coord.y > 0.430 && nut.coord.y != INVALID_COORD) { nut.coord.y = 0.430; }
    if (nut.coord.y < 0.250 && nut.coord.y != INVALID_COORD) { nut.coord.y = 0.250; }
    
    return nut;
}

/// Write data to the serial port.
/// Prepend the ID of the sender to the message.
/// Return the sent message.
String send_data(const String& data)
{
    String message = ID + data;
    Serial.println(message);
    return message;
}

String coord_to_string(const Coord& coord)
{
    return "X:" + String(coord.x) + ", Y:" + String(coord.y);
}

String nut_to_string(const Nut& nut)
{
    return "Type:" + String(nut.type) + ", " + coord_to_string(nut.coord);
}

bool check_for_start(String& msg)
{
    msg = get_data();
    bool start = msg == "START";
    if (start)
    {
        send_data("Starting the program.");
    }
    return start;
}

bool check_for_stop(const String& msg)
{
    bool stop = msg == "STOP";
    if (stop)
    {
        send_data("Stopping the program.");
    }
    return stop;
}

int parse_msg(const String& msg, Nut& nut)
{
    int result = 2;
    nut = parse_nut(msg);
    if (nut.coord.x != INVALID_COORD && nut.coord.y != INVALID_COORD && nut.type != INVALID_NUT_TYPE)
    {
        result = 1;
    }
    else if(check_for_stop(msg))
    {
        result = 0;
    }
    return result;
}
