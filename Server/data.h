#ifndef DATA_H
#define DATA_H

#include "globals.h"

#define USERS_DB "users.txt"
#define TRANSACTIONS_DB "transactions.txt"

typedef struct {
	int id;
	char firstName[21];
	char lastName[21];
	char pin[5];
	char dl[9];
	char ssn[10];
	char email[41];
	int balance;
} User;

User users[100];

typedef struct {
	int sockfd;
	User *user;
	int loginAttempts;
} Session;

Session *sessions[MAX_CLIENTS];

typedef struct {
	int userid;
	int transid;
	char desc[11];
	int amount;
} Transaction;

Transaction transactions[1000];

int numSessions;
int numUsers;
int numTransactions;

void readUsers();
int addNewUser(char *buffer);
int loginUser(int sockfd, char *buffer);
Session *sessionForSockfd(int sockfd);
void recordTransaction(int sockfd, const char *type, int amount);
int getTransactionsForUser(int sockfd, Transaction *tArray, int numRequested);
int logoutUser(int sockfd);

#endif