import serial
import time
import random

dev = "/dev/tty.usbmodem12341"
wave_event = 'WAV'
killed_event = 'HUMKIL'
beat_event = 'BEAT'
addr = '1'

ser = serial.Serial(dev)

ser.write('%s%s:%s\n' % (wave_event, addr, 2))
time.sleep(1)
ser.write('%s%s:%s\n' % (wave_event, addr, 2))
time.sleep(1)
ser.write('%s%s:%s\n' % (killed_event, addr, 2))
