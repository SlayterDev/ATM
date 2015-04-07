#include "data.h"
#include <signal.h>

void printUser(User user) {
	printf("User: %d %s %s %s %s %s %s %d\n", user.id, user.firstName, user.lastName,
				user.pin, user.dl, user.ssn, user.email, user.balance);
}

// Return a user struct from string
User userFromString(char *string, int newUser) {
	User user;
	char *tok = strtok(string, " "); // tok has user id
	if (!newUser)
		user.id = atoi(tok);
	else
		user.id = ++numUsers;

	tok = strtok(NULL, " "); // tok has first name
	strcpy(user.firstName, tok);

	tok = strtok(NULL, " "); // tok has last name
	strcpy(user.lastName, tok);

	tok = strtok(NULL, " "); // tok has pin
	strcpy(user.pin, tok);

	tok = strtok(NULL, " "); // tok has DL
	strcpy(user.dl, tok);

	tok = strtok(NULL, " "); // tok has SSN
	strcpy(user.ssn, tok);

	tok = strtok(NULL, " "); // tok has email
	strcpy(user.email, tok);

	if (!newUser) {
		tok = strtok(NULL, " "); // tok has balance
		user.balance = atoi(tok);
	} else {
		user.balance = 0;
	}

	if (DEBUG)
		printUser(user);

	return user;
}

// Return a transaction struct from a string
Transaction transFromString(char *buffer) {
	Transaction t;
	char *tok = strtok(buffer, " "); // user id
	t.userid = atoi(tok);

	tok = strtok(NULL, " "); // trans id
	t.transid = atoi(tok);

	tok = strtok(NULL, " "); // description
	strcpy(t.desc, tok);

	tok = strtok(NULL, " "); // amount
	t.amount = atoi(tok);

	return t;
}

Session *sessionForSockfd(int sockfd) {
	for (int i = 0; i < numSessions; i++) {
		if (sessions[i]->sockfd == sockfd)
			return sessions[i]; // Found the existing session
	}

	return NULL; // Session not yet started
}

void writeTransactions() {
	FILE *f = fopen(TRANSACTIONS_DB, "w");

	if (!f) {
		fprintf(stderr, "Could not open transaction database\n");
		exit(1);
	}

	for (int i = 0; i < numTransactions; i++) {
		// print each transaction to the file
		fprintf(f, "%d %d %s %d\n", transactions[i].userid, transactions[i].transid,
				transactions[i].desc, transactions[i].amount);
	}

	fclose(f);

	printf("Transactions written to database.\n");
}

void writeUsers() {
	FILE *f = fopen(USERS_DB, "w");

	if (!f) {
		fprintf(stderr, "Could not open users database.\n");
		exit(1);
	}

	for (int i = 0; i < numUsers; i++) {
		// print each user to the file
		fprintf(f, "%d %s %s %s %s %s %s %d\n", users[i].id, users[i].firstName, users[i].lastName,
				users[i].pin, users[i].dl, users[i].ssn, users[i].email, users[i].balance);
	}

	fclose(f);

	printf("Users written to database.\n");
	writeTransactions(); // Save the transactions
}

// Keyboard interrupt handler
void intHandler() {
	writeUsers(); // Save the data
	exit(0);
}

// See if a user already exists
int checkDuplicateUsers(char *buffer) {
	char first[21];
	char last[21];

	char *tok = strtok(buffer, " "); // tok holds 101
	tok = strtok(NULL, " "); // tok holds first name
	strcpy(first, tok);
	tok = strtok(NULL, " "); // tok holds last name
	strcpy(last, tok);

	for (int i = 0; i < numUsers; i++) {
		if (!strcmp(users[i].lastName, last) && !strcmp(users[i].firstName, first))
			return 0; // User already exists
	}

	return 1; // User does not exist
}

int addNewUser(char *buffer) {
	// Create a temp copy of buffer to not
	// overwrite the buffer
	char temp[strlen(buffer)+1];
	strcpy(temp, buffer);
	
	if (!checkDuplicateUsers(temp))
		return 0; // User already exists

	User u = userFromString(buffer, 1);
	users[numUsers-1] = u;
	printf("Adding ");
	printUser(u);
	
	return 1;
}

void readTransactions() {
	numTransactions = 0;
	if (access(TRANSACTIONS_DB, F_OK) != -1) {
		// Transaction database exists
		FILE *f = fopen(TRANSACTIONS_DB, "r");

		char buffer[150];
		while (fgets(buffer, 150, f) != NULL) {
			// trim newline
			buffer[strlen(buffer)-1] = '\0';

			// check for empty string
			if (strlen(buffer) <= 0)
				break;

			transactions[numTransactions++] = transFromString(buffer);
		}

		fclose(f);
	}
}

void readUsers() {
	numUsers = 0;
	numSessions = 0;
	if (access(USERS_DB, F_OK) != -1) {
		// User database exists
		FILE *f = fopen(USERS_DB, "r");

		char buffer[150];
		while (fgets(buffer, 150, f) != NULL) {
			// trim newline
			buffer[strlen(buffer)-1] = '\0';

			// check for empty string
			if (strlen(buffer) <= 0)
				break;

			users[numUsers++] = userFromString(buffer, 0);
		}
		printf("Num users: %d\n", numUsers);

		fclose(f);
	}

	readTransactions(); // Populate the transactions DB

	// Reegister a keyboard interrupt handler to safely
	// save the databases before the program terminates
	signal(SIGINT, intHandler);
}

int loginUser(int sockfd, char *buffer) {
	char first[21];
	char pin[5];

	char *tok = strtok(buffer, " "); // get rid of request code
	tok = strtok(NULL, " ");
	strcpy(first, tok);

	tok = strtok(NULL, " ");
	strcpy(pin, tok);

	int sessionFound = 0;
	Session *s = (Session *)malloc(sizeof(Session));
	for (int i = 0; i < numSessions; i++) {
		if (sessions[i]->sockfd == sockfd) {
			s = sessions[i];
			sessionFound = 1;
		}
	}

	if (!sessionFound) { // newSession
		s->sockfd = sockfd;
		s->loginAttempts = 0;
		sessions[numSessions] = s;
		numSessions++;
	}

	printf("Checking %s %s...\n", first, pin);
	for (int i = 0; i < numUsers; i++) {
		if (!strcmp(users[i].firstName, first) && !strcmp(users[i].pin, pin)) {
			// We got a user
			s->user = &users[i];
			s->loginAttempts = 0;

			return 205; // Login success
		} else if (!strcmp(users[i].firstName, first)) {
			s->loginAttempts++;
			if (s->loginAttempts >= 10) {
				// close the session
				free(s);

				// Rearange the array
				if (numSessions > 1) {
					for (int i = 0; i < numSessions; i++) {
						if (sessions[i] == NULL)
							sessions[i] = sessions[numSessions-1];
					}
				}

				numSessions--;

				return 204; // Login attempts exceeded
			}
		}
	}

	return 203; // Login failed
}

// Get the next id to assign to a new transaction
int getNextTransactionId(int userid) {
	int transid = 0;

	for (int i = 0; i < numTransactions; i++)  {
		if (transactions[i].userid == userid)
			transid++;
	}

	return transid;
}

void recordTransaction(int sockfd, const char *type, int amount) {
	Session *s = sessionForSockfd(sockfd);
	Transaction t;

	t.userid = s->user->id;
	t.transid = getNextTransactionId(t.userid);
	strcpy(t.desc, type);
	t.amount = amount;
	transactions[numTransactions] = t;
	numTransactions++;
}

int getTransactionsForUser(int sockfd, Transaction *tArray, int numRequested) {
	int numFound = 0;

	Session *s = sessionForSockfd(sockfd);
	int j = 0;
	for (int i = numTransactions-1; i >= 0; i--) {
		if (transactions[i].userid == s->user->id) {
			tArray[j] = transactions[i];
			j++;
			numFound++;

			if (numFound == numRequested)
				break;
		}
	}

	return numFound; // Real number of transactions
}

int logoutUser(int sockfd) {
	// close the session
	Session *s = sessionForSockfd(sockfd);

	if (!s)
		return 909; // Unknown error

	free(s);

	if (numSessions > 1) {
		// Rearrange the sessions array
		for (int i = 0; i < numSessions; i++) {
			if (sessions[i] == NULL)
				sessions[i] = sessions[numSessions-1];
		}	
	}

	numSessions--;

	return 803; // Logout success
}