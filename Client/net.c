#include <signal.h>
#include "net.h"

void sigtermHandler(int signum) {
	close(sockDesc);
	printf("[+] Socket closed\n");
	exit(0);
}

void debugPrints(const char *str) {
	if (DEBUG)
		printf("%s", str);
}

void initNet(int portNum) {
	sockDesc = socket(AF_INET, SOCK_STREAM, 0);
	if (sockDesc == -1) {
		fprintf(stderr, "[-] Could not create socket\n");
		exit(1);
	}
	debugPrints("[+] Socket Created\n");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);

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
		exit(1);
	}

	char serverReply[100];
	if (recv(sockDesc, serverReply, 100, 0) < 0) {
		fprintf(stderr, "[-] Failed to receive message\n");
		exit(1);
	}

	// Flush the buffer
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);

	//getchar();

	int resp = atoi(serverReply);

	return resp;
}

char *sendMessageWithResponse(char *message) {
	if (send(sockDesc, message, strlen(message), 0) < 0) {
		fprintf(stderr, "[-] Failed to send message\n");
		exit(1);
	}

	char serverReply[2000] = ""; // Can't be to careful
	if (recv(sockDesc, serverReply, 2000, 0) < 0) {
		fprintf(stderr, "[-] Failed to receive message\n");
		exit(1);
	}

	char *response = (char *)malloc(strlen(serverReply)+1);
	strcpy(response, serverReply);

	// Flush the buffer
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF);

	return response;
}