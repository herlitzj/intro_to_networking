import socket
import sys

def main():
	HOST = sys.argv[1]
	PORT = int(sys.argv[2])
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((HOST, PORT))
	s.sendall('Hello, world')
	data = s.recv(1024)
	s.close()
	print 'Received', repr(data)

main()