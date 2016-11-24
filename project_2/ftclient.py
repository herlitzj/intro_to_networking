import socket
import sys
MAX_MESSAGE_LENGTH = 2048

def get_directory_listing(socket):
	try:
		while(1):
			reply = socket.recv(MAX_MESSAGE_LENGTH)
			if "200 SUCCESS" in reply:
				print(reply.replace("200 SUCCESS", ""))
				return 0
			else:
				sys.stdout.write(reply)
				sys.stdout.flush()
	except:
		return 1


def get_file(socket):
	print("GET")
	socket.close()


def main():
	if(len(sys.argv) < 4):
		print("ftclient [host] [host port] [command] [client port]")
		sys.exit()

	HOST = sys.argv[1] if len(sys.argv) > 0 else "localhost"
	COMMAND_PORT = int(sys.argv[2] if len(sys.argv) > 1 else 31111)
	COMMAND = sys.argv[3] if len(sys.argv) > 2 else "-l"
	if(COMMAND == "-g"):
		FILE_NAME = sys.argv[4]
		DATA_PORT = int(sys.argv[5])
	else:
		DATA_PORT = int(sys.argv[4])

	command_socket = socket.create_connection((HOST, COMMAND_PORT))

	if COMMAND == "-l":
		command_socket.sendall(COMMAND)
		got_directory_listing = get_directory_listing(command_socket) == 0
		if(got_directory_listing):
			command_socket.close()
			sys.exit(0)
		else:
			print("Error getting directory listing")
			sys.exit(0)


	if COMMAND == "-g":
		get_file(command_socket)
		sys.exit(0)


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