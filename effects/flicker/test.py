import serial
import time
import random

dev = "/dev/tty.usbmodem12341"
start_event = 'START'
end_event = 'END'
beat_event = 'BEAT'
addr = '1'

ser = serial.Serial(dev)

#ser.write('%s%s:%s\n' % (beat_event, addr, 2))
ser.write('%s%s:%s\n' % (start_event, addr, 2))
