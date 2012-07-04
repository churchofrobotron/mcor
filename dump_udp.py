import sys
import select
import socket

def usage():
	print "Usage:"
	print " dump_udp <port>";

dump_hex = lambda x: " ".join([hex(ord(c))[2:].zfill(2) for c in x])

def main(argv=None):

	if(argv is None):
		argv = sys.argv

	if(len(argv) < 2):
		usage()
		sys.exit(1)

	port = int(argv[1])

	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	print "Opening port %d" %(port)
	s.bind(('127.0.0.1', port))
	s.setblocking(0)

	print "Waiting for data..."
	while True:
		result = select.select([s],[],[])
		msg = result[0][0].recv(20) # 10 bytes
		print "%s | %s" %(dump_hex(msg), msg)

if __name__ == "__main__":
	sys.exit(main())
