#include <signal.h>
#include "net.h"

// Keyboard Interrupt handler to properly close connection
// with server
void sigtermHandler(int signum) {
	close(sockDesc);
	printf("[+] Socket closed\n");
	exit(0);
}

void debugPrints(const char *str) {
	if (DEBUG)
		printf("[\033[31mDEBUG\033[0m]%s", str);
}

void initNet(int portNum, const char *host) {
	sockDesc = socket(AF_INET, SOCK_STREAM, 0);
	if (sockDesc == -1) {
		fprintf(stderr, "[-] Could not create socket\n");
		exit(1);
	}
	debugPrints("[+] Socket Created\n");

	if (host)
		server.sin_addr.s_addr = inet_addr(host);
	else
		server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);

	if (DEBUG) {
		printf("[\033[31mDEBUG\033[0m][+] Connecting to %s\n", 
				(host) ? host : "127.0.0.1");
	}

	if (connect(sockDesc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		fprintf(stderr, "[-] Could not connect to server\n");
		exit(1);
	}
	debugPrints("[+] Connected to server\n");

	// Setup SIGTERM handler to gracefully close socket
	signal(SIGINT, sigtermHandler);
}

// sendMessage assumes a null terminated string
int sendMessage(char *message) {
	if (send(sockDesc, message, strlen(message), 0) < 0) {
		fprintf(stderr, "[-] Failed to send message\n");
		close(sockDesc);
		exit(1);
	}

	char serverReply[100];
	if (recv(sockDesc, serverReply, 100, 0) < 0) {
		fprintf(stderr, "[-] Failed to receive message\n");
		close(sockDesc);
		exit(1);
	}

	// Flush the buffer
	// Sometimes garbage gets stuck in buffer when using the
	// tester program
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);

	// Convert response code into an int
	int resp = atoi(serverReply);

	return resp;
}

char *sendMessageWithResponse(char *message) {
	if (send(sockDesc, message, strlen(message), 0) < 0) {
		fprintf(stderr, "[-] Failed to send message\n");
		close(sockDesc);
		exit(1);
	}

	char serverReply[2000] = ""; // Can't be too careful
	if (recv(sockDesc, serverReply, 2000, 0) < 0) {
		fprintf(stderr, "[-] Failed to receive message\n");
		close(sockDesc);
		exit(1);
	}

	char *response = (char *)malloc(strlen(serverReply)+1);
	strcpy(response, serverReply);

	// Flush the buffer
	// Sometimes garbage gets stuck in buffer when using the
	// tester program
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);

	return response;
}