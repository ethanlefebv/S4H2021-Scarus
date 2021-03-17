#include <Arduino.h>

#ifndef comm_functions_h
#define comm_functions_h

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

/// Read data from the serial port.
/// Ignore the message if it was sent by this device.
/// Return the received data.
String get_data();

/// Parse a coordinate from a String.
/// Return the coordinate if it was valid, else
/// return (9999,9999).
Coord parse_coord(String data);

/// Parse and return a Nut from a String.
Nut parse_nut(String data);

/// Write data to the serial port.
/// Prepend the ID of the sender to the message.
/// Return the sent message.
String send_data(String data);

String coord_to_string(Coord coord);

String nut_to_string(Nut nut);

#endif
