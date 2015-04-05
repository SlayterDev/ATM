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

int addNewUser(char *buffer) {
	User u = userFromString(buffer, 1);
	users[numUsers-1] = u;
	printf("Adding ");
	printUser(u);
	
	return 1;
}

void readUsers() {
	numUsers = 0;
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

