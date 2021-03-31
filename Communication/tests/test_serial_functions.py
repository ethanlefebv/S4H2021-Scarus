import Communication.serial_functions.functions as ser_funcs
import fake_serial

def test_send_data():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "moma") == "0moma\n"
    return

def test_get_data():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.get_data(ser) != ""
    return

def test_coord_to_string():
    assert ser_funcs.coord_to_string(24, -42) == "24/-42"
    return

def test_nut_to_string():
    assert ser_funcs.nut_to_string(-46, 546, 1) == "/-46/546/1/"
    return
