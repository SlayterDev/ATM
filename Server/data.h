#ifndef DATA_H
#define DATA_H

#include "globals.h"

#define USERS_DB "users.txt"
#define TRANSACTIONS_DB "transactions.txt"

// Define a user
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

// User database in memory
User users[100];

// Define a Session with a client
typedef struct {
	int sockfd;
	User *user;
	int loginAttempts;
} Session;

// Sessions in memory
Session *sessions[MAX_CLIENTS];

// Define a transation
typedef struct {
	int userid;
	int transid;
	char desc[11];
	int amount;
} Transaction;

// Transactions database in memory
Transaction transactions[1000];

// Keeping track of counts
int numSessions;
int numUsers;
int numTransactions;

/*
 Read the users database from the file

 Called at startup
*/
void readUsers();

/*
 Add a new user to the database

 Takes a char pointer with all the user's info
*/
int addNewUser(char *buffer);

/*
 Attempt a login

 Takes the client socket descriptor and a char pointer
 containing the first name and pin of the user
*/
int loginUser(int sockfd, char *buffer);

/*
 Get a reference to the session corresponding to the passed in
 socket descriptor
*/
Session *sessionForSockfd(int sockfd);

/*
 Add a transaction to the database

 Takes the client socket descriptor, a char pointer to the type
 of transaction being recorded, and the amount of the transaction
*/
void recordTransaction(int sockfd, const char *type, int amount);

/*
 Return a list of transactions belonging to the user

 Takes the client socket descriptor, a pointer to store the 
 transactions to return, and the number of transactions the 
 client has requested

 The transactions are stored in the passed pointer. The function
 returns the actual number of transactions the function found.
 This may be lower than requested in the case that the user has
 less transactions than they requested.
*/
int getTransactionsForUser(int sockfd, Transaction *tArray, int numRequested);

/*
 Terminate the session with the user corresponding to the passed
 in socket descriptor.
*/
int logoutUser(int sockfd);

#endif