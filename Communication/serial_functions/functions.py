import serial
import argparse
encoding = "utf-8"
baudrate = 115200
ID = '0'

def print_received_data(data):
    print("Received from {0} : '{1}'\n".format(port, data))


def print_sent_data(data):
    print("Sent to {0} : '{1}'".format(port, data))


def get_data(ser):
    """Read data from ser (a serial port).

    Ignore the message if it was sent by this device.
    Return the received data.
    """
    tmp = ser.readline().decode(encoding).rstrip()
    data = ""
    # The first char is the ID of the sender: if the ID is this
    # device's ID, ignore the message
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


def send_coord(ser, x, y):
    """Write a 2D coordinate to ser (a serial port).

    Insert '|' between the coordinates to simplify the
    split operation when decoding.
    Return the sent message.
    """
    coord = "{0}|{1}".format(x, y)
    return send_data(ser, coord)


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

            print_sent_data(send_coord(ser, 42, 90))
            print_received_data(get_data(ser))
            print_sent_data(send_coord(ser, 0.12, -3.14))
            print_received_data(get_data(ser))
            print_sent_data(send_data(ser, "Hello World!"))
            print_received_data(get_data(ser))


            print_sent_data(send_data(ser, "STOP"))
            print_received_data(get_data(ser))

            print("Closing Serial communication with {0}.".format(port))
