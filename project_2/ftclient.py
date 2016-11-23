import socket
import sys

def main():
	if(len(sys.argv) < 4):
		print("ftclient [host] [host port] [command] [client port]")
		sys.exit()

	HOST = sys.argv[1] if len(sys.argv) > 0 else "localhost"
	SERVER_PORT = int(sys.argv[2] if len(sys.argv) > 1 else 31111)
	COMMAND = sys.argv[3] if len(sys.argv) > 2 else "-l"
	CLIENT_PORT = int(sys.argv[4] if len(sys.argv) > 3 else 31112)

	print("HOST: {0} S_PORT: {1} COMMAND: {2} C_PORT: {3}".format(HOST, SERVER_PORT, COMMAND, CLIENT_PORT))

	# server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# server.bind((HOST, PORT))
	# server.listen(1)
	# loop_counter = 0;
	# connection = None
	# address = None
	# client_handle = None
	# data = None
	
	
	# while 1:
	# 	try:
	# 		if connection is None:
	# 			print('The server is ready to receive')
	# 			connection, address = server.accept()
	# 			client_handle = connection.recv(MAX_MESSAGE_LENGTH).rstrip()
	# 			connection.sendall(HANDLE)
	# 			print('Connection made by {0} from {1} on port {2}'.format(client_handle, address[0], address[1]))
	# 		else:
	# 			data = connection.recv(MAX_MESSAGE_LENGTH)
	# 			if not data:
	# 				print("Connection closed by {0}".format(client_handle))
	# 				connection.close()
	# 				connection = None
	# 			else:
	# 				print("{0}: {1}".format(client_handle, data))
	# 				reply = raw_input('> ')
					
	# 				if reply == '\quit':
	# 					connection.close()
	# 					connection = None
	# 				else:
	# 					connection.sendall(reply)
	# 	except KeyboardInterrupt:
	# 		print("\nExiting Server")
	# 		sys.exit()	

main()