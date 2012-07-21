import sys
import select
import socket
import serial
import random
import time
from struct import *

serial_devices = []
last_time = time.time()

def find_devices():
   global serial_devices
   for dev in serial_devices:
      dev.close()
   serial_devices = []
   try:
      dev_name = "/dev/tty.usbmodem12341"
      serial_devices.append(serial.Serial(dev_name))
   except:
      print "Unable to open " + dev_name

def send_command(c):
   for dev in serial_devices:
      dev.write(c)

def send_laser(num):
   send_command("LAS1:" + "{0:x}\n".format(num))

def usage():
	print "Usage:"
	print " mcor-disatcher <port> (2084 of course bitz)";

def filter_space(value):
    if (value == 0x3A):
        return ' '
    else:
        return chr(value)

def get_initials(values, offset):
    return filter_space(values[offset]) + filter_space(values[offset+1]) + filter_space(values[offset+2])

def bcd_byte(value, mult):
    return (((value >> 4) * 10) + (value & 0xF)) * mult

def get_score(values, offset):
    b = (values[offset] & 0xF) * 1000000
    b = b + bcd_byte(values[offset+1], 10000)
    b = b + bcd_byte(values[offset+2], 100)
    b = b + bcd_byte(values[offset+3], 1)
    return b

def csv_it(values):
    res = ""
    for i in values:
        res = res + str(i) + ", "
    res = res[:len(res)-2]
    return res

def scoreboard_line(f, initials, score):
   if (len(initials.strip()) > 0):
      f.write(csv_it([initials, score, "deathface.png"])+"\n")

def parse_hex_initials(inits):
   letters = inits.split(" ")
   res = ""
   for i in letters:
      if (i != "3A"):
         res = res + chr(int("0x" + i, 0))
      else:
         res = res + " "
   return res

def parse_scoreboard(msg):
    # TODO: msg contains the initials and score of the latest player, rename last_death.gif to <initials>_<score>.gif
    # This may take a little more thought, we may need to avoid collisions of people with the same initials and score repeating?  Maybe we don't
    # care though, it's nice and stateless this way.

    f = open("/Users/bzztbomb/projects/churchOfRobotron/mame_146/nvram/robotron/nvram", "r")
    leaderboard = open("leaderboard/data/leaderboard_new.txt", "w")
    # Combine nibbles in values array
    values = []
    byte = f.read(2)
    while byte != "":
        b = unpack("BB", byte)
        values.append(((b[0] & 0xF) << 4) | (b[1] & 0xF))
        byte = f.read(2)

    # Spit out the most recent score first
    items = msg.split(",")
    scoreboard_line(leaderboard, parse_hex_initials(items[2]), items[3])

    # GC is stored a bit funny because you can optionally enter a 20 digit initial
    scoreboard_line(leaderboard, get_initials(values, 0x99), get_score(values, 0xB0))

    # Get rest of scores
    offset = 0xB4
    while (offset < 0x1FD):
       scoreboard_line(leaderboard, get_initials(values, offset), get_score(values, offset+3))
       offset = offset + 7
    leaderboard.close()
    f.close()
    print "Scoreboard written."

# We should save each death face to last_death.gif
def save_player_face():
    pass

dump_hex = lambda x: " ".join([hex(ord(c))[2:].zfill(2) for c in x])

def main(argv=None):

   parse_scoreboard("NewScores,Latest,41 3A 3A,18900")

   global last_time
   # TODO: Find devices during runtime?
   find_devices()

   if(argv is None):
      argv = sys.argv

   if(len(argv) < 2):
      usage()
      sys.exit(1)

   port = int(argv[1])

   gamerunning = False

   s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   print "Opening port %d" %(port)
   s.bind(('127.0.0.1', port))
   s.setblocking(0)

   print "Waiting for data..."
   while True:
      if (gamerunning):
         if (time.time() - last_time > 0.5):
            send_laser(random.randint(1,511));
            last_time = time.time()
      else:
         send_laser(0)

      result = select.select([s],[],[],0.001)
      if (len(result[0]) > 0):
         msg = result[0][0].recv(80) # 10 bytes
         print "%s | %s" %(dump_hex(msg), msg)

         if (msg.startswith("Game start")):
            gamerunning = True
         if (msg.startswith("Game over")):
            gamerunning = False
         if (msg.startswith("Player death")):
            save_player_face()
         if (msg.startswith("NewScores")):
            parse_scoreboard(msg)

if __name__ == "__main__":
   sys.exit(main())
