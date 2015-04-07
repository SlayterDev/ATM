ATM Client and Server Application for Group 5
=============================================


Compilation instructions:
- There are two directories, "Client" and "Server" containing the code for the corresponding application.

- Each directory contains a make file. Running "make" will compile the designated program and produce an executable named either "Client" or "Server"


Running instructions:
=====================

Server:
- Run the server using "./Server <port>" where <port> is the port number you'd like the server to run on (ex. ./Server 8000).

- The server will run without need for any further user interaction.

- Use ^C (control C) to quit the server application


Client:
- Run the client using "./Client <port> (host)" where port is the <port> number that the server is running on and (host) is the address of the server. The (host) parameter is optional and if not provided, the client will attempt to connect to "127.0.0.1".

- The client will then provide a basic user interface for the user to navigate the application. Follow the prompts given by the application.