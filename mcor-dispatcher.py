# Church of Robotron Event Dispatcher
import sys
import select
import socket
import serial
import random
import time
from struct import *
import subprocess
import os
import shutil

# CONFIG
scores_extension = ".gif"
mame_dir = "/Users/bzztbomb/projects/churchOfRobotron/mame_146/"
leaderboard_dir = "./leaderboard/"

#
serial_devices = []
last_time = time.time()

def usage():
	print "Usage:"
	print " mcor-disatcher <port> (2084 of course bitz)";

#
# Device interface
#
# TODO: Look at /dev/tty.* and try to open them
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

#
# Scoreboard
#
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
        res = res + str(i) + ","
    res = res[:len(res)-1]
    return res

def scoreboard_line(f, initials, score):
   if (len(initials.strip()) > 0):
      f.write(csv_it([initials, score, initials.strip() + "_" + str(score) + scores_extension])+"\n")

def parse_hex_initials(inits):
   if (inits.strip() == "FF FF FF"):
      return "NOOB"
   letters = inits.split(" ")
   res = ""
   for i in letters:
      if (i != "3A"):
         res = res + chr(int("0x" + i, 0))
      else:
         res = res + " "
   return res

def parse_scoreboard(msg):
    f = open(os.path.join(mame_dir, "nvram/robotron/nvram"), "r")

    leaderboard_data = os.path.join(leaderboard_dir, "data")
    leaderboard_photocapture = os.path.join(leaderboard_dir, "photo_capture")

    leaderboard = open(os.path.join(leaderboard_data, "leaderboard.txt"), "w")

    # Combine nibbles in values array
    values = []
    byte = f.read(2)
    while byte != "":
        b = unpack("BB", byte)
        values.append(((b[0] & 0xF) << 4) | (b[1] & 0xF))
        byte = f.read(2)

    # Spit out the most recent score first
    items = msg.split(",")
    recent_initials = (parse_hex_initials(items[2])).strip()
    recent_score = int(items[3])
    scoreboard_line(leaderboard, recent_initials, recent_score)

    source = os.path.join(leaderboard_photocapture, "deathface" + scores_extension)
    dest = os.path.join(leaderboard_data, recent_initials.strip() + "_" + str(items[3]) + scores_extension)
    try:
       shutil.move(source, dest)
    except:
       print "Error moving" + source + " to " + dest
       pass

    # GC is stored a bit funny because you can optionally enter a 20 digit initial
    savior_initials = get_initials(values, 0x99).strip()
    savior_score = get_score(values, 0xB0)
    scoreboard_line(leaderboard, savior_initials, savior_score)

    # Get rest of scores
    offset = 0xB4
    while (offset < 0x1FD):
       scoreboard_line(leaderboard, get_initials(values, offset), get_score(values, offset+3))
       offset = offset + 7
    leaderboard.close()
    f.close()

    cwd = os.getcwd()
    os.chdir(leaderboard_dir)
    subprocess.call(['./copy_and_save_leaderboard.sh'])
    os.chdir(cwd)

    print "Scoreboard written."

    if ((savior_score == recent_score) and (savior_initials == recent_initials)):
       return True
    else:
       return False

# We should save each death face to last_death.gif
capture_handle = None

def start_capture():
   global capture_handle
   cwd = os.getcwd()
   os.chdir(os.path.join(leaderboard_dir, "photo_capture"))
   cmd = "/usr/bin/gst-launch -vt videotestsrc ! video/x-raw-yuv ! jpegenc ! image/jpeg,width=(int)320,height=(int)240,framerate=(fraction)5/1,pixel-aspect-ratio=(fraction)1/1 ! multifilesink location=work/output-%05d.jpeg"
   capture_handle = subprocess.Popen(cmd.split(" "))
   os.chdir(cwd)
   print "Capture started."

def stop_capture():
   global capture_handle
   if (capture_handle == None):
      return
   capture_handle.kill()
   capture_handle = None
   print "Capture ended."

def save_player_face():
   stop_capture()
   cwd = os.getcwd()
   os.chdir(os.path.join(leaderboard_dir, "photo_capture"))
   print "Saving deathface"
   subprocess.call(['/bin/sh', 'make-deathface.sh'])
   os.chdir(cwd)
   start_capture()

dump_hex = lambda x: " ".join([hex(ord(c))[2:].zfill(2) for c in x])

def main(argv=None):
   global last_time
   # TODO: Find devices during runtime?
   find_devices()

   if(argv is None):
      argv = sys.argv

   if(len(argv) < 2):
      usage()
      sys.exit(1)

   port = int(argv[1])

   dump_udp = False
   if (len(argv) > 2):
      dump_udp = (int(argv[2]) == 1)

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

         if (dump_udp):
            print "%s | %s" %(dump_hex(msg), msg)

         if (msg.startswith("Game start")):
            gamerunning = True
            start_capture()
         if (msg.startswith("Game over")):
            gamerunning = False
            stop_capture()
         if (msg.startswith("Player death")):
            save_player_face()
         if (msg.startswith("NewScores")):
            if (parse_scoreboard(msg)):
               print "NEW MUTANT SAVIOR!"
         if (msg.startswith("WaveNum")):
            # Watchpoint based events need gamerunning check
            if (gamerunning):
               wave_num = int(msg.split(":")[1])+1
               print "New Wave: " + str(wave_num)
         if (msg.startswith("ScoreChange")):
            # Watchpoint based events need gamerunning check
            if (gamerunning):
               score = msg.split(",")[1]
               # print score
         if (msg.startswith("HumanSaved")):
            print "Human Saved!  Praise the Mutant!"
         if (msg.startswith("HumanKilled")):
            print "Human Killed!"
         if (msg.startswith("GruntKilledByElectrode")):
            print "Stupid Robot"
         if (msg.startswith("EnforcerShot")):
            # Watchpoint based events need gamerunning check
            if (gamerunning):
               print "Enforcer shot!"

if __name__ == "__main__":
   sys.exit(main())
