import serial_functions.functions as ser_funcs
import fake_serial

def test_send_data():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "moma") == "0moma"
    return

def test_get_data():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.get_data(ser) != ""
    return

def test_send_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_coord(ser, 24, -42) == "024|-42"
    return
