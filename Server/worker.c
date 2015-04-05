#include "worker.h"
#include "net.h"
#include "data.h"

int getCode(char *message) {
	char code[4];
	int i;
	for (i = 0; i < 3; i++)
		code[i] = message[i];
	code[i] = '\0';

	return atoi(code);
}

int processRequest(int sockfd, char *message) {
	int reqCode = getCode(message);

	switch (reqCode) {
		case 101:
			if (addNewUser(message))
				writeToClient(sockfd, "104");
			else
				writeToClient(sockfd, "103");
			break;
		case 201:
			printf("Log in requested...\n");
			writeToClient(sockfd, "205");
			break;
		default:
			break;
	}
}