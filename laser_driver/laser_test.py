import serial
import time
import random

laser_dev="/dev/tty.usbmodem12341"
laser_prefix="LAS1:"

ser=serial.Serial(laser_dev)

#just do some random stuff
while True:
  ser.write(laser_prefix+"{:x}\n".format(random.randint(1,511)))
  time.sleep(0.1)


