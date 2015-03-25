#include "view.h"
#include "net.h"

#define ACCOUNT_BUFFER_SIZE 111

void createAccount() {
	char message[ACCOUNT_BUFFER_SIZE];
	
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