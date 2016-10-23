import socket
import sys

def main():
	HOST = ''
	PORT = int(sys.argv[1] if len(sys.argv) > 1 else 31111)
	server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	server.bind((HOST, PORT))
	server.listen(1)
	handle = raw_input('What is your handle: ')
	print('The server is ready to receive')
	
	while 1:
		connection, address = server.accept()
		print('Connection made from {0}'.format(address))
		data = connection.recv(1024)
		
		if not data:
			break

		print("{0}: {1}".format(handle, data))
		reply = raw_input('> ')
		connection.sendall(reply)
		connection.close()

main()