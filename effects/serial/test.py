import serial
import time
import random

dev = "/dev/tty.usbmodem12341"
start_event = 'START'
end_event = 'END'
addr = '1'

ser = serial.Serial(dev)

while True:
  ser.write('%s%s:%s\n' % (start_event, addr, 2))
  time.sleep(1)


