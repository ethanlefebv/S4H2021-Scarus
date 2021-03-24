import serial
import argparse
import time
encoding = "utf-8"
baudrate = 115200
ID = '0'

def print_received_data(data):
    print("Received : '{0}'\n".format(data))


def print_sent_data(data):
    print("Sent : '{0}'".format(data))


def get_data(ser):
    """Read data from ser (a serial port).

    Ignore the message if it was sent by this device.
    Return the received data.
    """
    tmp = ser.readline().decode(encoding).rstrip()
    data = ""
    # The first char is the ID of the sender: if the ID is this
    # device's ID, ignore the message
    if tmp != "":
        if tmp[0] != ID:
            data = tmp[1:]
    return data


def send_data(ser, data):
    """Write data to ser (a serial port).

    Prepend the ID of the sender to the message.
    Return the sent message.
    """
    message = "{0}{1}".format(ID, data)
    ser.write(message.encode(encoding))
    return message


def coord_to_string(x, y):
    """Convert a 2D coordinate to a string.

    Insert '|' between the coordinates to simplify the
    split operation when decoding.
    Return the string.
    """
    return "{0}|{1}".format(x, y)


def nut_to_string(x, y, type):
    """Convert a Nut to a string.

    Insert '/' between the type and coordinate to simplify
    the split operation when decoding.
    Return the string.
    """
    return "{0}/{1}".format(type, coord_to_string(x, y))

def wait_for_data(ser, check):
    data = None
    #while data != check:
    time.sleep(0.1)
    data = get_data(ser)
    print_received_data(data)
    return data


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', required=True, help='Port used for serial communication.')
    parser.add_argument('-d', action='store_true', help='Run demo program.')
    args = parser.parse_args()
    port = args.p

    if args.d:
        print("Opening Serial communication with {0}.".format(port))
        with serial.Serial(port, baudrate) as ser:
            ser.flush()
            print_received_data(get_data(ser))

            print_sent_data(send_data(ser, "START"))
            print_received_data(get_data(ser))

            print_sent_data(send_data(ser, nut_to_string(0, 12, 34)))
            print_received_data(get_data(ser))
            print_sent_data(send_data(ser, nut_to_string(1, -56, -78)))
            print_received_data(get_data(ser))
            print_sent_data(send_data(ser, "Hello World!"))
            print_received_data(get_data(ser))


            print_sent_data(send_data(ser, "STOP"))
            print_received_data(get_data(ser))

            print("Closing Serial communication with {0}.".format(port))
