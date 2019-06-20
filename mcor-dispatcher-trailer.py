#!/usr/bin/env python3
# vim: set expandtab ts=3 sw=3:

import glob
import os
import serial
import struct
import sys
import time
import datetime
import math
import shutil
from picamera import PiCamera
from PIL import Image
from io import BytesIO

DEV_FPGA_PORT = "/dev/ttyS0"
DEV_TTY_PREFIX = "/dev/ttyACM*" if not os.path.isfile("dev-mode") else "/dev/tty.usbmodem*"
if (os.path.isfile("dev-mode")):
   print("Override config for development.")
SERIAL_SCAN_INTERVAL = 20.0
BEAT_INTERVAL = 5
EXTRA_MUTANT = 25000
SECONDS_TO_CAPTURE = 4.0
POST_GAME_OVER_SECONDS = 3.0
FRAMES_PER_SECOND = 4.0
CAPTURE_DELAY = 1.0 / FRAMES_PER_SECOND
NUM_PHOTO_FRAMES = FRAMES_PER_SECOND * SECONDS_TO_CAPTURE
LEADERBOARD_DIR = "./leaderboard/"
GIF_FRAME_DURATION = math.floor(CAPTURE_DELAY * 1000 * 0.5)
ALTAR_ID = "TEARDOWN"

class GameState():
   def __init__(self):
      self.game_running = False
      self.serial_devices = []
      self.next_life = EXTRA_MUTANT
      self.current_score = 0
      self.capturing = False
      self.camera = None
      self.last_capture = None
      self.deathface_frames = []


# Device code
def find_devices(old_devices):
   for dev in old_devices:
      try:
         dev.close()
      except:
         print("Error closing device")
   ret = []
   potential_devices = glob.glob(DEV_TTY_PREFIX)
   for dev_name in potential_devices:
      try:
         ret.append(serial.Serial(dev_name))
      except:
         print("Unable to open " + dev_name)
   return ret

def send_command(c, game_state):
   for dev in game_state.serial_devices:
      dev.write(c.encode())

# Death face routines
def start_capture(game_state):
   if game_state.camera is None:
      game_state.camera = PiCamera()
      game_state.camera.resolution = (320, 208)
   game_state.capturing = True
   game_state.last_capture = None

def capture_if_needed(game_state):
   if game_state.capturing == False:
      return
   if game_state.last_capture != None and time.time() - game_state.last_capture < CAPTURE_DELAY:
      return

   print("Capturing")
   game_state.last_capture = time.time()
   stream = BytesIO()
   game_state.camera.capture(stream, format='bgr', use_video_port=True)
   stream.seek(0)
   game_state.deathface_frames.append(Image.frombytes("RGB", (320, 208), stream.read(-1)))
   stream.close()
   if len(game_state.deathface_frames) > NUM_PHOTO_FRAMES:
      del game_state.deathface_frames[0]

def end_capture(game_state):
   game_state.capturing = False

def save_player_face(game_state):
   print("Saving deathface")
   leaderboard_photocapture = os.path.join(LEADERBOARD_DIR, "photo_capture")
   face_path = os.path.join(leaderboard_photocapture, "deathface.gif")
   # TODO: Look into PICamera's circular buffers.
   if len(game_state.deathface_frames) > 0:
      game_state.deathface_frames[0].save(face_path, save_all=True, append_images=game_state.deathface_frames[1:], duration=GIF_FRAME_DURATION, loop=0)
      game_state.deathface_frames = []
   else:
      print("No frames, no fame!")
   print("Save death face complete")

# Scoreboard
def save_scoreboard(initials, score):
   leaderboard_photocapture = os.path.join(LEADERBOARD_DIR, "photo_capture")
   source = os.path.join(leaderboard_photocapture, "deathface.gif")
   leaderboard_data = os.path.join(LEADERBOARD_DIR, "data")
   # TODO: Deal with binary initials, switch : to space.
   if initials == b'\x01\x00\x00' or initials == None:
      initials = 'NOOB'
   else:
      initials = bytes.decode(initials, 'utf-8')
   basename = initials.strip() + "_" + str(score) + "_" + datetime.datetime.now().isoformat() + "_" + ALTAR_ID
   dest = os.path.join(leaderboard_data, basename + ".gif")
   try:
      shutil.move(source, dest)
   except Exception as e:
      print(e)
      print("Error moving" + source + " to " + dest)
      # Notify scoreboard that there is a new score.
   with open(os.path.join(leaderboard_data, basename + ".new"), "w"):
      pass

# Event routines
def score_bcd_to_int(bcd_int):
   try:
      return int('%x' % bcd_int)
   except:
      return None

# Deal with non-blocking serial port.
def read_bytes(s, to_read):
   ret = []
   while len(ret) < to_read:
      ret.extend(s.read(to_read - len(ret)))
   return bytearray(ret)

def event_nvram_dump(s, game_state):
   data = read_bytes(s, 7)
   score, initials = struct.unpack('>I3s', data)
   score = score_bcd_to_int(score)
   print('nvram_dump=%s,%s' % (score, initials))
   save_scoreboard(initials, score)

def event_wave_change(s, game_state):
   data = read_bytes(s, 1)
   wave = struct.unpack('>B', data)[0]
   print('wave_change=%d' % wave)
   if game_state.game_running:
      send_command("WaveNum1\n", game_state)

def event_enforcer_shot(s, game_state):
   if game_state.game_running:
      send_command("LAS1:G\n", game_state)
   print('enforcer_shot')

def event_score_changed(s, game_state):
   data = read_bytes(s, 4)
   print(data)
   # score = struct.unpack('>I', data)[0]
   game_state.current_score = score_bcd_to_int(struct.unpack('>I', data)[0])
   if game_state.current_score > game_state.next_life:
      game_state.next_life = game_state.next_life + EXTRA_MUTANT
      send_command("XP\n", game_state)
      print("Extra mutant!!")
   print('score_changed=%s' % game_state.current_score)

def event_game_over(s, game_state):
   print('game_over')
   send_command("GameOver\n", game_state)
   send_command("END1:666\n", game_state) # Not sure if this is still needed??
   # Get deathface
   start = time.time()
   while time.time() - start < POST_GAME_OVER_SECONDS:
      capture_if_needed(game_state)
      time.sleep(CAPTURE_DELAY * 0.5)
   save_player_face(game_state)
   end_capture(game_state)
   print('game over complete')

def event_grunt_killed_by_electrode(s, game_state):
#   print('grunt_killed_by_electrode')
   pass

def event_human_killed(s, game_state):
   print('human_killed')
   send_command("HumanKilled\n", game_state)

def event_human_saved(s, game_state):
   print('human_saved')

def event_still_trying(s, game_state):
   print('still_trying')

def event_player_death(s, game_state):
   send_command("PlayerDeath\n", game_state) # Death whistle!!
   print('player_death')

def event_game_start(s, game_state):
   game_state.game_running = True
   send_command("GameStart\n", game_state)
   print('game_start')

def event_boot(s, game_state):
   print('boot')

handler_map = {
   0xfb: event_nvram_dump,
   0xfa: event_wave_change,
   0xf9: event_enforcer_shot,
   0xf8: event_score_changed,
   0xf7: event_game_over,
   0xf6: event_grunt_killed_by_electrode,
   0xf5: event_human_killed,
   0xf4: event_human_saved,
   0xf3: event_still_trying,
   0xf2: event_player_death,
   0xf1: event_game_start,
   0xf0: event_boot,
}

# POST
def test_command(state, cmd, text = None):
   print(cmd if text is None else text)
   send_command(cmd + "\n", state)
   time.sleep(1)

def power_on_self_test():
   test_state = GameState()
   print("Camera test")
   start_capture(test_state)
   start_time = time.time()
   while time.time() - start_time < 2:
      capture_if_needed(test_state)
   end_capture(test_state)
   save_player_face(test_state)
   save_scoreboard(b"AZY", 100)
   return
   test_state.serial_devices = find_devices([])
   print("POWER ON TEST, Devices Found: " + str(len(test_state.serial_devices)))
   test_command(test_state, "GameStart")
   test_command(test_state, "HumanKilled", text = "Flappers")
   test_command(test_state, "PlayerDeath", text = "Death Whistle")
   test_command(test_state, "WaveNum1", text = "Puffers")
   test_command(test_state, "LAS1:G", text = "Lasers")
   test_command(test_state, "XP", text = "Knockers")
   test_command(test_state, "GameOver")
   test_command(test_state, "END1:666")

# Main loop
def main(argv=None):
   #power_on_self_test()
   #exit()

   # Setup
   try:
      game_serial = serial.Serial(DEV_FPGA_PORT, 115200, timeout=0.01)
   except:
      print("Unable to find FPGA serial port!!")
      game_serial = None

   # Main loop!
   game_state = GameState()
   last_beat = None # send heartbeat to lazers so they don't stay on forever.
   capture_score_game_start = False # Deal with case where game starts before intials are entered

   serial_scan = time.time() - SERIAL_SCAN_INTERVAL - 1.0 # Force serial scan
   while True:
      # Look for new devices
      if time.time() - serial_scan >= SERIAL_SCAN_INTERVAL:
         game_state.serial_devices = find_devices(game_state.serial_devices)
         print("Found devices: " + str(len(game_state.serial_devices)))
         serial_scan = time.time()

      # Look for new events
      if game_serial:
         c = game_serial.read(1)
         if len(c):
            c = c[0]
            if c in handler_map:
               handler_map[c](game_serial, game_state)

      # Deal with game state changes
      if game_state.game_running:
         if start_time is None:
            # Check to see if we should save the last score to the scoreboard
            # before resetting state.  This can happen if the player hits start
            # before the enter initials screen is complete.
            if capture_score_game_start:
               save_scoreboard(None, game_state.current_score)
            capture_score_game_start = True

            start_time = time.time()
            last_beat = time.time() - BEAT_INTERVAL
            game_state.current_score = 0
            game_state.next_life = EXTRA_MUTANT
            start_capture(game_state)
         else:
            capture_if_needed(game_state)
            if time.time() - last_beat > 5:
               print("send beat")
               send_command("BEAT1:" + "{0:x}\n".format(int(time.time() - start_time)), game_state)
               last_beat = time.time()
      else:
         start_time = None
         last_beat = None

if __name__ == "__main__":
   sys.exit(main())
