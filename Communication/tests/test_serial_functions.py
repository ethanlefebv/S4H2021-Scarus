import Communication.serial_functions.functions as ser_funcs
import fake_serial

# send_data() function's tests (les reverefier)

def test_send_data_string_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "moma") == "0moma\n"
    return

def test_send_data_int_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "886") == "0886"
    return

def test_send_data_float_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "8.86") == "08.86"
    return

def test_send_data_void_return():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.send_data(ser, "") == "0"
    return

def test_send_data_string_serial():
    ser = fake_serial.FakeSerial()
    ser_funcs.send_data(ser, "moma")
    assert ser.readline() == "0moma"
    return

def test_send_data_int_serial():
    ser = fake_serial.FakeSerial()
    ser_funcs.send_data(ser, "886")
    assert ser.readline() == "0886"
    return

def test_send_data_float_serial():
    ser = fake_serial.FakeSerial()
    ser_funcs.send_data(ser, "8.86")
    assert ser.readline() == "08.86"
    return

def test_send_data_void_serial():
    ser = fake_serial.FakeSerial()
    ser_funcs.send_data(ser, "")
    assert ser.readline() == "0"
    return

# Tests pour la fonction get_data()

def test_get_data_void():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.get_data(ser) != ""
    return

def test_get_data_string():
    ser = fake_serial.FakeSerial()
    ser_funcs.send_data(ser, "Salut moi c'est tony" )
    assert ser_funcs.get_data(ser) == "Salut moi c'est tony"
    return

def test_get_data_wrong_ID():
    ser = fake_serial.FakeSerial()
    ser_funcs.send_data(ser, "0Salut moi c'est tony")
    assert ser_funcs.get_data(ser) == "Salut moi c'est tony"
    return

def test_get_data_right_ID():
    ser = fake_serial.FakeSerial()
    ser_funcs.send_data(ser, "1Salut moi c'est tony")
    assert ser_funcs.get_data(ser) == "Salut moi c'est tony"
    return

# coord_to_string function's tests

def coord_to_string_int():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string(24, 16) == "24|16"
    return

def coord_to_string_negatives():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string(-24, -16) == "-24|-16"
    return

def coord_to_string_void():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string("", "") == "|"
    return

def coord_to_string_string():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string("hgfds", "gfds") == "hgfds|gfds"
    return

def coord_to_string_float():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.coord_to_string(8.84, 0.76) == "8.84|0.76"
    return

# nut_to_string function's tests

def nut_to_string_int_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, 24, 16) == "0/24|16"
    return

def nut_to_string_negatives_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, -24, -16) == "0/-24|-16"
    return

def nut_to_string_void_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string("", "") == "0/|"
    return

def nut_to_string_string_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, "hgfds", "gfds") == "0/hgfds|gfds"
    return

def nut_to_string_float_coord():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0, 8.84, 0.76) == "0/8.84|0.76"
    return

def nut_to_string_int_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(1, 24, 16) == "1/24|16"
    return

def nut_to_string_negative_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(-1, 24, 16) == "-1/24|16"
    return

def nut_to_string_void_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string("", 24, 16) == "/24|16"
    return

def nut_to_string_string_type():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string("TonysanUWU", 24, 16) == "TonysanUWU/24|16"
    return

def nut_to_string_float_typ():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.nut_to_string(0.0, 24, 16) == "0.0/24|16"
    return

# print_received_data function's tests (a reverifier)

def print_received_data_int():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.print_received_data(ser_funcs.send_data(ser, "1/16|24"))
    return

# print_sent_data function's tests (a reverifier)

def print_sent_data_int():
    ser = fake_serial.FakeSerial()
    assert ser_funcs.print_sent_data(ser_funcs.send_data(ser, "1/16|24"))
    return

