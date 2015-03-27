#include "view.h"
#include "net.h"

#define ACCOUNT_BUFFER_SIZE 111
#define LOGIN_BUFFER_SIZE 30
#define AMOUNT_BUFFER_SIZE 15
#define TRANSACTION_BUFFER_SIZE 6

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
	printf("Enter amount you\'d like to withdraw\n");
	printf("Enter $0 to cancel: $");

	int amount;
	scanf("%d", &amount);

	if (amount == 0) {
		system("clear");
		return;
	}

	sprintf(message, "401 %d", amount);

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
			printf("[-] Your balance is $%s\n\n", tok);
			break;
		default:
			break;
	}

	free(resp); // Free pointer from sendMessage

}

void deposit() {
	system("clear");

	char message[AMOUNT_BUFFER_SIZE] = "";

	printf("Enter amount you\'d like to deposit\n");
	printf("Enter $0 to cancel: $");

	int amount;
	scanf("%d", &amount);

	if (amount == 0) {
		system("clear");
		return;
	}

	sprintf(message, "301 %d", amount);

	// TODO: Check if ATM can take it ;)

	char *resp = sendMessageWithResponse(message);
	char *tok = strtok(resp, " ");

	int respCode = atoi(tok); // Get the response code
	tok = strtok(NULL, " ");

	int i = 0;
	while (isdigit(tok[i++])); // strtok() gives garbage sometimes
	tok[i] = '\0';

	system("clear");
	switch (respCode) {
		case 303:
			printf("[+] Deposit successful!\n");
			printf("[+] Your balance is now $%s\n\n", tok);
			break;
		case 304:
			printf("[-] Deposit failed\n");
			printf("[-] Your balance is $%s\n\n", tok);
			break;
		default:
			break;
	}

	free(resp); // free the response
}

void showBalance() {
	char message[] = "501";

	char *resp = sendMessageWithResponse(message);
	char *tok = strtok(resp, " ");

	int respCode = atoi(tok);
	tok = strtok(NULL, " "); // tok now has balance

	system("clear");
	if (respCode == 503) {
		printf("[+] Your balance is $%s\n\n", tok);
	} else {
		printf("[-] An unknown error has occured\n\n");
	}

	free(resp); // free response from server
}

void showTransactions() {
	system("clear");

	char message[TRANSACTION_BUFFER_SIZE] = "";

	printf("Enter number of transactions to show: ");
	int numTransactions;
	scanf("%d", &numTransactions);
	sprintf(message, "601 %d", numTransactions);

	char *resp = sendMessageWithResponse(message);
	char *tok = strtok(resp, " "); // response code
	int respCode = atoi(tok);

	if (respCode == 603) {
		// show transactions
		tok = strtok(NULL, " "); // number transactions returned
		int numReturned = atoi(tok);

		printf("\n\nShowing last %d transactions:\n\n", numReturned);

		int i;
		printf("id Description     Amount\n");
		printf("=========================\n");
		for (i = 0; i < numReturned; i++) {
			// Jank hacky way to get the data
			printf("%-2s %-15s $%s\n", strtok(NULL, " "), strtok(NULL, " "),
								strtok(NULL, " "));
		}

		printf("\n\nPress enter to continue...");
		int enter = 0;
		while (enter != '\r' && enter != '\n') { 
			enter = getchar(); 
		}

		system("clear");
	} else {
		system("clear");
		printf("[-] An unknown error has occured\n\n");
	}
}

void buyStamps() {
	system("clear");

	printf("How many stamps would you like to buy? ($1 each): ");

	int numStamps;
	scanf("%d", &numStamps);

	char message[AMOUNT_BUFFER_SIZE] = "";
	sprintf(message, "701 %d", numStamps);

	// TODO: check # of stamps in machine

	char *resp = sendMessageWithResponse(message);
	char *tok = strtok(resp, " ");
	int respCode = atoi(tok); // response code

	system("clear");
	if (respCode == 704) {
		// success
		tok = strtok(NULL, " ");
		printf("[+] Your stamps are being dispensed\n");
		printf("[+] Your balance is now $%s\n\n", tok);
	} else if (respCode == 703) {
		printf("[-] You don't have enough funds\n\n");
	} else {
		printf("[-] An unknown error has occured\n\n");
	}
}

void logout() {
	char message[] = "801";

	int resp = sendMessage(message);

	system("clear");
	if (resp == 803) {
		printf("[+] You have been logged out\n");
		printf("[+] Thank you for using our ATM\n\n");
	} else {
		printf("[-] An unknown error has occured\n\n");
	}
}

void userMenu() {
	char c = '1';

	while (c != '6') {
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
			case '2':
				deposit();
				break;
			case '3':
				showBalance();
				break;
			case '4':
				showTransactions();
				break;
			case '5':
				buyStamps();
				break;
			case '6':
				logout();
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
				c = '2';
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