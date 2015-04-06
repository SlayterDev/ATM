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

void depositCash(int sockfd, char *message) {
	char *tok = strtok(message, " "); // trim the request code
	tok = strtok(NULL, " "); // tok has amount

	int amount = atoi(tok);
	Session *s = sessionForSockfd(sockfd);

	char response[20] = "";
	if (s) {
		s->user->balance += amount;
		sprintf(response, "303 %d", s->user->balance);
		recordTransaction(sockfd, "Deposit", amount);
	} else {
		sprintf(response, "304 %d", 0);
	}

	writeToClient(sockfd, response);
}

void withdrawCash(int sockfd, char *message) {
	char *tok = strtok(message, " "); // strip the request
	tok = strtok(NULL, " "); // tok has amount

	int amount = atoi(tok);
	Session *s = sessionForSockfd(sockfd);

	char response[20] = "";
	if (s) {
		if (s->user->balance >= amount) {
			s->user->balance -= amount;
			sprintf(response, "403 %d", s->user->balance);
			recordTransaction(sockfd, "Withdraw", amount);
		} else {
			sprintf(response, "404 %d", s->user->balance);
		}
	}

	writeToClient(sockfd, response);
}

void showTransactions(int sockfd, char *message) {
	char *tok = strtok(message, " "); // strip request
	tok = strtok(NULL, " "); // tok has # transactions

	int numTransactions = atoi(tok);

	if (numTransactions > 5)
		numTransactions = 5;
	else if (numTransactions < 1)
		numTransactions = 1;

	Transaction *transactions = (Transaction *)malloc(sizeof(transactions)*numTransactions);

	numTransactions = getTransactionsForUser(sockfd, transactions, numTransactions);

	char response[110] = "";
	if (numTransactions > 0) {
		char temp[20];
		sprintf(temp, "603 %d ", numTransactions);
		strcat(response, temp);
		for (int i = 0; i < numTransactions; i++) {
			sprintf(temp, "%d %s %d", transactions[i].transid,
					transactions[i].desc, transactions[i].amount);
			strcat(response, temp);

			if (i < numTransactions-1)
				strcat(response, " ");
		}
	} else {
		sprintf(response, "909");
	}

	writeToClient(sockfd, response);
}

void buyStamps(int sockfd, char *message) {
	char *tok = strtok(message, " "); // strip request
	tok = strtok(NULL, " "); // number of stamps

	int numStamps = atoi(tok);
	Session *s = sessionForSockfd(sockfd);
	char response[20] = "";
	if (s) {
		if (s->user->balance >= numStamps) {
			s->user->balance -= numStamps;
			recordTransaction(sockfd, "Stamps", numStamps);
			sprintf(response, "704 %d", s->user->balance);
		} else {
			sprintf(response, "703");
		}
	} else {
		sprintf(response, "909");
	}

	writeToClient(sockfd, response);
}

int processRequest(int sockfd, char *message) {
	int reqCode = getCode(message);

	char response[100] = "";
	Session *s;
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
			break;
		case 302:
			// Too much cash
			printf("An ATM has reported that the machine is full\n");
			s = sessionForSockfd(sockfd);

			if (s)
				sprintf(response, "305 %d", s->user->balance);
			else
				sprintf(response, "305 %d", 0);

			writeToClient(sockfd, response);
			break;
		case 401:
			withdrawCash(sockfd, message);
			break;
		case 402:
			// Empty ATM
			printf("An ATM is out of cash\n");
			writeToClient(sockfd, "405");
			break;
		case 501:
			// get balance
			s = sessionForSockfd(sockfd);

			if (s) {
				sprintf(response, "503 %d", s->user->balance);
			} else {
				sprintf(response, "909");
			}

			writeToClient(sockfd, response);
			break;
		case 601:
			showTransactions(sockfd, message);
			break;
		case 701:
			buyStamps(sockfd, message);
			break;
		case 702:
			printf("An ATM is out of stamps\n");
			sprintf(response, "705");
			writeToClient(sockfd, response);
			break;
		case 801:
			sprintf(response, "%d", logoutUser(sockfd));
			writeToClient(sockfd, response);
			break;
		default:
			break;
	}

	return 0;
}