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

///// Parse a coordinate from a String.
///// Return the coordinate if it was valid, else
///// return the INVALID coord.
//Coord parse_coord(const String& data)
//{
//    Coord coord;
//    int separator_index = data.indexOf('|');
//    if (separator_index != -1)
//    {
//        String x = data.substring(0, separator_index);
//        String y = data.substring(separator_index + 1);
//        coord.X = x.toInt();
//        coord.Y = y.toInt();
//    }
//    else
//    {
//        coord.X = INVALID_COORD;
//        coord.Y = INVALID_COORD;
//    }
//    return coord;
//}

///// Parse and return a Nut from a String.
//Nut parse_nut(const String& data)
//{
//    Nut nut;
//    int separator_index = data.indexOf('/');
//    if (separator_index != -1)
//    {
//        String type = data.substring(0, separator_index);
//        String coord_string = data.substring(separator_index + 1);
//        nut.type = type.toInt();
//        nut.coord = parse_coord(coord_string);
//    }
//    else
//    {
//        nut.type = INVALID_NUT_TYPE;
//        nut.coord = parse_coord("");
//    }
//    return nut;
//}
//
//String parse_data(const String& data, int& intData)
//{
//    int front_separator_index = data.indexOf('/');
//    String nextSubString = data.substring(separator_index+1);
//    Serial.println(nextSubString);
//    int back_separator_index = nextSubString.indexOf('/')+1;
//    
//    if (front_separator_index != -1 && back_separator_index != -1)
//    {
//        intData = data.substring(front_separator_index+1, back_separator_index).toInt();
//        Serial.println(data.substring(back_separator_index));
//        return  data.substring(back_separator_index);
//    }
//    else
//    {
//        Serial.println("Error: Communication")
//        intData = INVALID_INPUT;
//        return String("error");
//    }
//}

String parse_data(const String& data, int& intData)
{
    int front_separator_index = data.indexOf('/');
    String nextSubString = data.substring(front_separator_index+1);
    int back_separator_index = nextSubString.indexOf('/')+1;
    
    if (front_separator_index != -1 && back_separator_index != -1)
    {
        intData = data.substring(front_separator_index+1, back_separator_index).toInt();
        return  data.substring(back_separator_index);
    }
    else
    {
        Serial.println("Error: Communication");
        intData = INVALID_INPUT;
        return String("error");
    }
}

/// Parse and return a Nut from a String.
Nut parse_nut(const String& data)
{
    Nut nut;
    int nutX;
    int nutY;
    parse_data(parse_data(parse_data(data, nutX), nutY), nut.type);

    if(nutX == INVALID_INPUT)
    {
        nut.coord.x = INVALID_COORD;
    }
    else
    {
       nut.coord.x =  nutX/1000.0;
    }
    
    if(nutY == INVALID_INPUT)
    {
        nut.coord.y = INVALID_COORD;
    }
    else
    {
       nut.coord.y =  nutY/1000.0;
    }

    if(nut.type == INVALID_INPUT)
    {
        nut.type = INVALID_NUT_TYPE;
    }

    // Slide bed limit dimensions
    if(nut.coord.x > 0.180 && nut.coord.x != INVALID_COORD){nut.coord.x = 0.180;}
    if(nut.coord.x < 0.015 && nut.coord.x != INVALID_COORD){nut.coord.x = 0.015;}
    
    if(nut.coord.y > 0.430 && nut.coord.y != INVALID_COORD){nut.coord.y = 0.430;}
    if(nut.coord.y < 0.250 && nut.coord.y != INVALID_COORD){nut.coord.y = 0.250;}
    
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
    if (msg == "START")
    {
        send_data("Starting the program.");
        return true;
    }
    return false;
}

bool check_for_stop(const String& msg)
{
    if (msg == "STOP")
    {
        send_data("Stopping the program.");
        
        return true;
    }
    return false;
}

int parse_msg(const String& msg, Nut& nut)
{
    nut = parse_nut(msg);
    if (nut.coord.x != INVALID_COORD && nut.coord.y != INVALID_COORD && nut.type != INVALID_NUT_TYPE)
    {
        return 1;
    }
    else if(check_for_stop(msg))
    {
        return 0;
    }
    return 2;  
}
