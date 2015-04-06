#include "data.h"
#include <signal.h>

void printUser(User user) {
	printf("User: %d %s %s %s %s %s %s %d\n", user.id, user.firstName, user.lastName,
				user.pin, user.dl, user.ssn, user.email, user.balance);
}

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

	tok = strtok(NULL, " ");
	strcpy(user.pin, tok);

	tok = strtok(NULL, " ");
	strcpy(user.dl, tok);

	tok = strtok(NULL, " ");
	strcpy(user.ssn, tok);

	tok = strtok(NULL, " ");
	strcpy(user.email, tok);

	if (!newUser) {
		tok = strtok(NULL, " ");
		user.balance = atoi(tok);
	} else {
		user.balance = 0;
	}

	printUser(user);

	return user;
}

Session *sessionForSockfd(int sockfd) {
	for (int i = 0; i < numSessions; i++) {
		if (sessions[i]->sockfd == sockfd)
			return sessions[i];
	}

	return NULL;
}

void writeUsers() {
	FILE *f = fopen(USERS_DB, "w");

	if (!f) {
		fprintf(stderr, "Could not open users database.\n");
		exit(1);
	}

	for (int i = 0; i < numUsers; i++) {
		fprintf(f, "%d %s %s %s %s %s %s %d\n", users[i].id, users[i].firstName, users[i].lastName,
				users[i].pin, users[i].dl, users[i].ssn, users[i].email, users[i].balance);
	}

	fclose(f);

	printf("Users written to database.\n");
}

void intHandler() {
	writeUsers();
	exit(0);
}

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
			return 0;
	}

	return 1;
}

int addNewUser(char *buffer) {
	if (!checkDuplicateUsers(buffer))
		return 0; // User already exists

	User u = userFromString(buffer, 1);
	users[numUsers-1] = u;
	printf("Adding ");
	printUser(u);
	
	return 1;
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
		sessions[numSessions] = s;
		numSessions++;
	}

	printf("Checking %s %s...\n", first, pin);
	for (int i = 0; i < numUsers; i++) {
		if (!strcmp(users[i].firstName, first) && !strcmp(users[i].pin, pin)) {
			// We got a user
			s->user = &users[i];
			s->loginAttempts = 0;

			return 205;
		} else {
			s->loginAttempts++;
			if (s->loginAttempts >= 10) {
				// close the session
				free(s);

				// Rearange the array
				for (int i = 0; i < numSessions; i++) {
					if (sessions[i] == NULL)
						sessions[i] = sessions[numSessions-1];
				}

				numSessions--;

				return 204;
			}
		}
	}

	return 203;
}