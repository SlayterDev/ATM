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

int depositCash(int sockfd, char *message) {
	char *tok = strtok(message, " "); // trim the request code
	tok = strtok(NULL, " "); // tok has amount

	int amount = atoi(tok);

	Session *s = sessionForSockfd(sockfd);

	char response[20] = "";
	if (s) {
		s->user->balance += amount;
		sprintf(response, "303 %d", s->user->balance);
	} else {
		sprintf(response, "304 %d", 0);
	}

	writeToClient(sockfd, response);

	return 0;
}

int processRequest(int sockfd, char *message) {
	int reqCode = getCode(message);

	char response[100] = "";
	switch (reqCode) {
		case 101:
			if (addNewUser(message))
				writeToClient(sockfd, "104");
			else
				writeToClient(sockfd, "105");
			break;
		case 201:
			printf("Log in requested...\n");
			int ret = loginUser(sockfd, message);
			if (ret == 205)
				writeToClient(sockfd, "205");
			else if (ret == 203)
				writeToClient(sockfd, "203");
			else
				writeToClient(sockfd, "204");
			break;
		case 301:
			depositCash(sockfd, message);
		default:
			break;
	}

	return 0;
}