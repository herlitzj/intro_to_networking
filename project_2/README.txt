#Acknowledgements
The server is written in C (ftserver.c) and was mostly copied from a prior assignemtn for OS class, which was in turn copied heavily from Beej's guide.

The client is written in python (ftclient.py) and is mostly an extended version of the server I wrote for the prior assignment with some additions based on Google searches and stackoverflow posts.

#Prep, etc.
The only prep required is compiling the server. Simply use gcc. E.g. "gcc ftserver.c -o ftserver" (without the quotes)

All testing of my code was done on flip1

#Server
To run the compiled server simply run it and input the port number you wish to run the server on. E.g. "./ftserver 30123" (without the quotes)

The server will run in a loop waiting for connections. When it gets a connection it will fork a child process and then deal with the request. If it gets a -l command, it will send a directory listing. If it gets a -g command it check for the file, and if it is found, it will send the file through the socket. If not, it will send an error message.

You can quit the server with a ctrl-c SIGINT;

#Client
To run client, just execute it with python the proper usage pattern. E.g. "python ftclient.py 30123 flip1.engr.oregonstate.edu -l 30124" (without the quotes).

The client will check for proper usage and if not found will print proper usage and quit.