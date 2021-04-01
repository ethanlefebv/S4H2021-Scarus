#ifndef comm_functions_h
#define comm_functions_h

class String;
const int INVALID_INPUT = 9999;

struct Coord
{
    float x = INVALID_INPUT;
    float y = INVALID_INPUT;
};

struct Nut
{
    Coord coord;
    int type = INVALID_INPUT;
    bool is_valid = false;
};

/// Read data from the serial port.
/// Ignore the message if it was sent by this device.
/// Return the received data.
String get_data();

/// Parse an int from a String.
/// Return the read value with the param int_data.
String parse_data(const String& data, int& int_data);

/// Parse and return a Nut from a String.
Nut parse_nut(const String& data);

/// Write data to the serial port.
/// Prepend the ID of the sender to the message.
/// Return the sent message.
String send_data(const String& data);

String coord_to_string(const Coord& coord);

String nut_to_string(const Nut& nut);

bool should_start(const String& msg);

bool should_stop(const String& msg);

#endif
