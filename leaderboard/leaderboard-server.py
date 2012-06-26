from SimpleHTTPServer import SimpleHTTPRequestHandler
import urlparse
import time
import sys

if __name__ == '__main__':
	from BaseHTTPServer import HTTPServer
	if sys.argv[1:]:
		port = int(sys.argv[1])
	else:
		port = 8000

	# Only bind on loopback.  This requires the leaderboard server to be running 
	# on the same machine physically connected to the projector.
	# Change as needed.
	server_address = ('127.0.0.1', port)

	server = HTTPServer(server_address, SimpleHTTPRequestHandler)
	print('Starting server on port %d, use <Ctrl-C> to stop' %(port))
	server.serve_forever()
