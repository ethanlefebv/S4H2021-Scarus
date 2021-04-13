from init_stop.init_stop_sequences import *
import Communication.tests.fake_serial as fake_ser

def init_fake_sequence():
    camera_index, port = init_parse()
    cam = init_camera(camera_index)
    ser = init_fake_port(port)
    init_opencr(ser)
    return cam, port, ser

def init_fake_port(port):
    print("Opening Serial communication with {0}.".format(port))
    ser = fake_ser.FakeSerial()
    ser.flush()
    return ser