import serial
import time


def get_line_msg(ser):
    return ser.readline().decode('ansi')


def get_msg(ser):
    return ser.read().decode('ansi')


class CachedPort:
    ports = {}

    def __init__(self, func) -> None:
        self.func = func

    def __call__(self, *args):
        if args[0] in CachedPort.ports:
            return CachedPort.ports[args[0]]
        CachedPort.ports[args[0]] = self.func(*args)
        return CachedPort.ports[args[0]]


@CachedPort
def setup_arduino_port(port, baud=115200, timeout=1):
    ser = serial.Serial(port, baud, timeout=timeout)

    # Arduino will send back "Arduino setup finished!" once it's all set
    while True:
        msg = get_line_msg(ser)
        if not msg.strip():
            time.sleep(1)
        else:
            if msg.find('Arduino') + 1:
                break
    return ser
