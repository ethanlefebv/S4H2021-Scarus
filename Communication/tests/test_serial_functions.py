from Communication.serial_functions import functions as ser_funcs
import fake_serial

# get_data() tests

def test_get_data_void():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.get_data(ser) != ""
    return

def test_get_data_string():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.get_data(ser) == "Hello World!"
    return

# send_data() tests

def test_send_data_string_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "moma") == "0moma\n"
    return

def test_send_data_int_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "886") == "0886\n"
    return

def test_send_data_float_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "8.86") == "08.86\n"
    return

def test_send_data_void_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "") == "0\n"
    return

# coord_to_string tests

def test_coord_to_string_int():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string(24, 16) == "24/16"
    return

def test_coord_to_string_negatives():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string(-24, -16) == "-24/-16"
    return

def test_coord_to_string_void():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string("", "") == "/"
    return

def test_coord_to_string_string():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string("hgfds", "gfds") == "hgfds/gfds"
    return

def test_coord_to_string_float():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string(8.84, 0.76) == "8.84/0.76"
    return

# nut_to_string tests

def test_nut_to_string_int_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, 24, 16) == "/0/24/16/"
    return

def test_nut_to_string_negatives_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, -24, -16) == "/0/-24/-16/"
    return

def test_nut_to_string_void_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string("", "", "") == "////"
    return

def test_nut_to_string_string_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, "hgfds", "gfds") == "/0/hgfds/gfds/"
    return

def test_nut_to_string_float_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, 8.84, 0.76) == "/0/8.84/0.76/"
    return

def test_nut_to_string_int_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(1, 24, 16) == "/1/24/16/"
    return

def test_nut_to_string_negative_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(-1, 24, 16) == "/-1/24/16/"
    return

def test_nut_to_string_void_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string("", 24, 16) == "//24/16/"
    return

def test_nut_to_string_string_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string("TonysanUWU", 24, 16) == "/TonysanUWU/24/16/"
    return

def test_nut_to_string_float_typ():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0.0, 24, 16) == "/0.0/24/16/"
    return
