import sys
import select
import socket
from struct import *

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

def parse_scoreboard(msg):
    # TODO: msg contains the initials and score of the latest player, rename last_death.gif to <initials>_<score>.gif
    # This may take a little more thought, we may need to avoid collisions of people with the same initials and score repeating?  Maybe we don't
    # care though, it's nice and stateless this way.

    f = open("/Users/bzztbomb/projects/churchOfRobotron/mame_146/nvram/robotron/nvram", "r")
    # Combine nibbles in values array
    values = []
    byte = f.read(2)
    while byte != "":
        b = unpack("BB", byte)
        values.append(((b[0] & 0xF) << 4) | (b[1] & 0xF))
        byte = f.read(2)

    # TODO: Spit out a scores.txt that contains csv of initials, score, and <initials>_<score>.gif

    # GC is stored a bit funny because you can optionally enter a 20 digit initial
    print get_initials(values, 0x99)
    print get_score(values, 0xB0)

    # Get rest of scores
    offset = 0xB4
    while (offset < 0x1FD):
        print get_initials(values, offset)
        print get_score(values, offset+3)
        offset = offset + 7
    f.close()

# We should save each death face to last_death.gif
def save_player_face():
    pass

dump_hex = lambda x: " ".join([hex(ord(c))[2:].zfill(2) for c in x])

def main(argv=None):

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
      result = select.select([s],[],[])
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
