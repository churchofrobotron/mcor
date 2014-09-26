import serial
import time
import random

laser_dev="/dev/tty.usbmodem12341"
laser_prefix="LAS1:"

ser=serial.Serial(laser_dev)

#just do some random stuff
while True:
  for i in range(9):
    ser.write(laser_prefix+"{0:x}\n".format(1<<i))
    time.sleep(0.01)


