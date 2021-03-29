#include <Arduino.h>
#include "comm_functions.h"

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

/// Parse a coordinate from a String.
/// Return the coordinate if it was valid, else
/// return the INVALID coord.
Coord parse_coord(const String& data)
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
        coord.X = INVALID_COORD;
        coord.Y = INVALID_COORD;
    }
    return coord;
}

/// Parse and return a Nut from a String.
Nut parse_nut(const String& data)
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
        nut.type = INVALID_NUT_TYPE;
        nut.coord = parse_coord("");
    }
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
    return "X:" + String(coord.X) + ", Y:" + String(coord.Y);
}

String nut_to_string(const Nut& nut)
{
    return "Type:" + String(nut.type) + ", " + coord_to_string(nut.coord);
}
