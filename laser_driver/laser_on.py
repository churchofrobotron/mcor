import serial
import time
import random

laser_dev="/dev/tty.usbmodem12341"
laser_prefix="LAS1:"
beat_prefix="BEAT1:"

ser=serial.Serial(laser_dev)

#just do some random stuff
while True:
  ser.write(laser_prefix+"g\n")
  time.sleep(0.1)


