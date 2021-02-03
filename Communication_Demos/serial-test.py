import serial
encoding = "utf-8"

def get_message(comm):
    received = comm.readline().decode(encoding).rstrip()
    print("Received from Arduino : '{0}'\n".format(received))

def send_message(comm, message):
    print("Sending to Arduino : '{0}'".format(message))
    comm.write(message.encode(encoding))

print("Opening Serial communication with Arduino.")
with serial.Serial('/dev/ttyACM0', 9600) as ser:
    ser.flush()

    get_message(ser)

    print("Starting communication with Arduino.")
    send_message(ser, "START")
    get_message(ser)
    send_message(ser, "Hello World!")
    get_message(ser)
    print("Stopping communication with Arduino.")
    send_message(ser, "STOP")
    get_message(ser)

    print("Closing Serial communication with Arduino.")
