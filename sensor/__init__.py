import os
import serial
import threading
import subprocess


class AppleMotionSensor(object):
    def __init__(self):
        self._coords = (0, 0, 0)
        self._stop_event = threading.Event()

    def get_coords(self):
        return self._coords

    def start(self):
        self._stop_event.clear()
        self._thread = threading.Thread(target=self._read_from_sensor)
        self._thread.start()

    def stop(self):
        self._stop_event.set()
        self._thread.join()

    def _read_from_sensor(self):
        cmd_dir = os.path.join(os.path.dirname(__file__), 'macbook')
        cmd_str = os.path.join(cmd_dir, 'AMSTracker') + ' -s -u 0.01'
        cmd = subprocess.Popen(cmd_str, stdout=subprocess.PIPE, shell=True)
        while not self._stop_event.is_set():
            line = cmd.stdout.readline()
            self._update_sensor_data(line)
        if not cmd.returncode:
            cmd.terminate()

    def _update_sensor_data(self, line):
        try:
            values = line.rstrip("\n").split()
            normalize = lambda x: 0.5 + (float(x) / 128.0)
            self._coords = map(normalize, values)
        except ValueError:
            pass




class ArduinoMotionSensor(object):
    '''
    Class to get access to arduino based accelerometer controller.  Reads
    sensor putput from serial port in a background thread and allows accessing
    the latest sample via self.last_sample, or self.x, self.y, self.z
    properties from the main thread without ever having to risk blocking on
    a serial read operation.
    '''

    def __init__(self, serial_port, baud_rate=115200, timeout=1):
        self.last_sample = (0, 0, 0)
        self.io = serial.Serial(serial_port, baud_rate, timeout=timeout)

    @property
    def x(self):
        return self.last_sample[0]

    @property
    def y(self):
        return self.last_sample[1]

    @property
    def z(self):
        return self.last_sample[2]

    def get_coords(self):
        return (self.x, self.y, self.z)

    def reset_connection(self):
        self.io.write("die\n")
        self.io.flush()
        self.io.write("live\n")
        self.io.flush()

    def start(self):
        '''starts reading sensor output from serial interface inside thread'''
        self._stop_reading = threading.Event()
        self._read_thread = threading.Thread(target=self._read_loop)
        self._read_thread.start()

    def stop(self):
        '''
        signals the background thread to stops reading from serial interface
        and tells the controller to stop writing output
        '''
        if self._read_thread.is_alive():
            self._stop_reading.set()
            self._read_thread.join()
        self.io.write("die\n")
        self.io.flush()

    def _read_loop(self):
        '''
        internal method started in a background thread by the start_reading
        method.  it will just keep reading the sensor data and update the
        samples we can just access from our main thread
        '''
        self.reset_connection()
        while not self._stop_reading.is_set():
            try:
                line = self.io.readline()
                values = line.strip().split(',')
                normalize = lambda x: float(x) / 1024.0
                last_sample = map(normalize, values)
            except:
                return
            self.last_sample = last_sample
