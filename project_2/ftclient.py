import socket
import sys
import time
import os
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


def get_file(socket, file_name):
	print("GETTING")
	local_file_name = file_name
	file_exists = os.path.isfile(file_name)

	if(file_exists):
		split_name = file_name.split(".")
		local_file_name = split_name[0] + "-" + str(int(time.time())) + "." + split_name[1]

	while(1):
		reply = socket.recv(MAX_MESSAGE_LENGTH)
		if "400 FILE NOT FOUND" in reply:
			print("File: {0} not found on server".format(file_name))
			return 0
		elif "200 SUCCESS" in reply:
			with open(local_file_name, "a") as file:
				file.write(reply.replace("200 SUCCESS", ""))
			return 0
		else:
			with open(local_file_name, "a") as file:
				file.write(reply);


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
			command_socket.close()
			sys.exit(0)


	if COMMAND == "-g":
		command_socket.sendall(COMMAND)
		if("200" in command_socket.recv(MAX_MESSAGE_LENGTH)):
			print("OK TO SEND FILENAME")
			command_socket.sendall("{0}|{1}".format(FILE_NAME, DATA_PORT))
			got_file = get_file(command_socket, FILE_NAME)
			if got_file == 0:
				print("GOT IT")
				command_socket.close()
				sys.exit(0)
			else:
				print("DIDNT GET IT")
				command_socket.close()
				sys.exit(0)
		else:
			print("DIDNT WORK")
			command_socket.close()
			sys.exit(0)


main()