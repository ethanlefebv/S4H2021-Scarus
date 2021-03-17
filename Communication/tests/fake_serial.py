class FakeSerial:
    """A mock-up of the Serial class. Useful for testing."""

    def __init__(self, port='COM1', baudrate=19200, timeout=1):
        self.name = port
        self.port = port
        self.timeout = timeout
        self.baudrate = baudrate
        self._isOpen = True
        self._encoding = "utf-8"
        self._receivedData = b""
        self._data = b"Hello World!\nSecond sentence.\n0450|-42\n"

    def isOpen(self):
        return self._isOpen

    def open(self):
        self._isOpen = True

    def close(self):
        self._isOpen = False

    def write(self, string):
        self._receivedData += string
        self._data += string

    def read(self, n=1):
        string = self._data[0:n]
        self._data = self._data[n:]
        return string

    def readline(self):
        returnIndex = self._data.decode(self._encoding).find("\n")
        string = b""
        if returnIndex != -1:
            string = self._data[0:returnIndex+1]
            self._data = self._data[returnIndex+1:]
        return string

    def flush(self):
        self._receivedData = b""
        self._data = b""

