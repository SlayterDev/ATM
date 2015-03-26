#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void serverLoop(int portNum) {
	int socketDesc, clientSock, c, readSize;
	struct sockaddr_in server, client;
	char clientMessage[2000];

	socketDesc = socket(AF_INET, SOCK_STREAM, 0);
	if (socketDesc == -1) {
		fprintf(stderr, "[-] Could not create socket\n");
		exit(1);
	} else {
		printf("[+] Socket creted\n");
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(portNum);

	if (bind(socketDesc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		fprintf(stderr, "[-] Could not bind socket\n");
		exit(1);
	}
	printf("[+] Socket binded\n");

	listen(socketDesc, 3);

	printf("[+] Waiting for connections\n");
	c = sizeof(struct sockaddr_in);

	clientSock = accept(socketDesc, (struct sockaddr *)&client, (socklen_t *)&c);
	if (clientSock < 0) {
		fprintf(stderr, "[-] Connect failed\n");
		exit(1);
	}
	printf("[+] Connection accepted\n");

	char serverMessage[256];
	while((readSize = recv(clientSock , clientMessage , 2000 , 0)) > 0) {
        //Send the message back to client
        printf("Recieved: %s\n> ", clientMessage);
        memset(serverMessage, 0, sizeof(serverMessage)); // clear server buffer
        fgets(serverMessage, 256, stdin);

        // trim trailing newline
        if (serverMessage[strlen(serverMessage)-1] == '\n')
        	serverMessage[strlen(serverMessage)-1] = '\0';

        send(clientSock , serverMessage , strlen(serverMessage), 0);
        memset(clientMessage, 0, sizeof(clientMessage)); // clear client buffer
    }
}

void clientLoop(int portNum) {
	int sock;
	struct sockaddr_in server;
	char message[1000], serverReply[2000];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		fprintf(stderr, "[-] Could not create socket\n");
		exit(1);
	}
	printf("[+] Socket created\n");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		fprintf(stderr, "[-] Could not connect to server\n");
		exit(1);
	}
	printf("[+] Connected to server\n");

	while (strcmp(message, "quit\n") != 0) {
		memset(message, 0, sizeof(message));
		printf("send> ");
		fgets(message, 1000, stdin);

		if (send(sock, message, strlen(message), 0) < 0) {
			fprintf(stderr, "[-] Failed to send message\n");
			break;
		}

		if (recv(sock, serverReply, 2000, 0) < 0) {
			fprintf(stderr, "[-] Failed to recieve message\n");
			break;
		}

		printf("Server: %s\n", serverReply);
		memset(serverReply, 0, sizeof(serverReply));
	}

	close(sock);
}

int main(int argc, char const *argv[]) {
	if (argc < 3 || argc > 3) {
		fprintf(stderr, "Usage: %s <port> <--server or --client>\n", argv[0]);
		fprintf(stderr, "Use --server to act as the server.\n");
		fprintf(stderr, "Use --client to act as the client.\n");
		exit(1);
	}

	int portNum = atoi(argv[1]);
	
	if (!strcmp(argv[2], "--server")) {
		serverLoop(portNum);
	} else if (!strcmp(argv[2], "--client")) {
		clientLoop(portNum);
	} else {
		fprintf(stderr, "Usage: %s <port> <--server or --client>\n", argv[0]);
		fprintf(stderr, "Use --server to act as the server.\n");
		fprintf(stderr, "Use --client to act as the client.\n");
		exit(1);
	}

	return 0;
}