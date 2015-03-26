#include "view.h"
#include "net.h"

#define ACCOUNT_BUFFER_SIZE 111
#define LOGIN_BUFFER_SIZE 30
#define AMOUNT_BUFFER_SIZE 15

void createAccount() {
	char message[ACCOUNT_BUFFER_SIZE] = "";
	
	system("clear");
	
	strcat(message, "101 "); // Tell server to create account

	char buffer[41]; // Largest piece of data is 40 chars
	
	printf("First Name (20 chars max): ");
	scanf("%s", buffer);
	strcat(message, buffer); // add first name
	strcat(message, " ");

	printf("Last Name (20 chars max): ");
	scanf("%s", buffer);
	strcat(message, buffer); // add last name
	strcat(message, " ");

	printf("Pin Number (4 digits 0-9): ");
	scanf("%s", buffer);
	strcat(message, buffer); // add pin number
	strcat(message, " ");

	printf("DL Number (8 digits 0-9): ");
	scanf("%s", buffer);
	strcat(message, buffer); // add DL number
	strcat(message, " ");

	printf("SSN (9 digits 0-9): ");
	scanf("%s", buffer);
	strcat(message, buffer); // add SSN
	strcat(message, " ");

	printf("Email Address (40 chars max): ");
	scanf("%s", buffer);
	strcat(message, buffer); // add email

	int resp = sendMessage(message);

	system("clear");
	switch (resp) {
		case 103:
			printf("[-] Invalid info given. Please try again.\n\n");
			break;
		case 104:
			printf("[+] Account created successfully!\n\n");
			break;
		case 105:
			printf("[-] This account already exists. Please try again.\n\n");
			break;
		case 908:
			printf("[-] Some info was missing. Please try again.\n\n");
			break;
		default:
			printf("[-] An unknown error has occured. (Error code: %d)\n\n", resp);
			break;
	}
}

void withdraw() {
	system("clear");

	char message[AMOUNT_BUFFER_SIZE] = "";

	printf("Withdrawing...\n");
	printf("Enter amount you'd like to withdraw\n");
	printf("Enter $0 to cancel: $");

	int amount;
	scanf("%d", &amount);

	if (amount == 0) {
		system("clear");
		return;
	}

	sprintf(message, "401 %d", amount);
	printf("%s\n", message);
	
	// TODO: Check ATM has enough cash

	char *resp = sendMessageWithResponse(message);
	char *tok = strtok(resp, " ");

	int respCode = atoi(tok);  // Get the response code
	tok = strtok(NULL, " ");   // tok now has the balance

	int i = 0;
	while (isdigit(tok[i++])); // Shouldn't have to do this
	tok[i] = '\0';			   // but strtok() keeps giving garbage

	system("clear");
	switch (respCode) {
		case 403:
			printf("[+] Your cash is being dispensed\n");
			printf("[+] Your balance is now $%s\n\n", tok);
			break;
		case 404:
			printf("[-] You do not have enough funds\n");
			printf("[-] Your balance is now $%s\n\n", tok);
			break;
		default:
			break;
	}

	free(resp); // Free pointer from sendMessage

}

void userMenu() {
	char c = '1';

	while (c != '3') {
		printf("Welcome...\n");
		printf("1. Withdraw\n");
		printf("2. Deposit\n");
		printf("3. Show Balance\n");
		printf("4. Show transactions\n");
		printf("5. Buy stamps\n");
		printf("6. Logout\n");
		printf("\nEnter your selection then press enter: ");

		scanf("%c", &c);

		switch (c) {
			case '1':
				withdraw();
				break;
			case '6':
				return;
				break;
			default:
				system("clear");
				printf("Invalid input (%c) please try again\n\n", c);
				break;
		}
	}
}

void login() {
	system("clear");

	char message[LOGIN_BUFFER_SIZE] = "";

	strcat(message, "201 ");

	char buffer[21] = "";
	char firstName[21] = "";
	printf("Enter first name: ");
	scanf("%s", buffer);
	strcpy(firstName, buffer);
	strcat(message, buffer); // add first name
	strcat(message, " ");

	printf("Enter pin: ");
	scanf("%s", buffer);
	strcat(message, buffer); // add pin number

	int resp = sendMessage(message);

	system("clear");
	switch (resp) {
		case 203:
			printf("[-] Authentication failure. Try again.\n\n");
			break;
		case 204:
			printf("[-] Too many failed login attempts.\n");
			printf("[-] Terminating...\n");
			close(sockDesc);
			exit(1);
			break;
		case 205:
			printf("[+] Authentication successful!\n");
			printf("[+] Welcome, %s\n\n", firstName);
			userMenu();
			break;
	}
}

void welcome() {
	char c = '1';

	while (c != '3') {
		printf("Welcome...\n");
		printf("1. Create Account\n");
		printf("2. Login with Pin\n");
		printf("3. Quit\n");
		printf("\nEnter your selection then press enter: ");

		scanf("%c", &c);

		switch (c) {
			case '1':
				createAccount();
				break;
			case '2':
				login();
				break;
			case '3':
				return;
				break;
			default:
				system("clear");
				printf("Invalid input (%c) please try again\n\n", c);
				break;
		}
	}
}