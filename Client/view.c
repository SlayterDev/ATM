#include "view.h"
#include "net.h"

void createAccount() {
	char message[] = "101 Brad Slayter 1234 12345678 1234567890 brad@unt.edu";
	int resp = sendMessage(message);
	printf("Received response: %d\n", resp);

	printf("Press enter to continue: ");
	scanf("%d", &resp);
}

void welcome() {
	char c = '1';

	while (c != '3') {
		printf("Welcome...\n");
		printf("1. Create Account\n");
		printf("2. Login with Pin\n");
		printf("3. Quit\n");
		printf("\nEnter your selection then press enter: ");

		c = getchar();

		switch (c) {
			case '1':
				createAccount();
				break;
			case '3':
				return;
			default:
				system("clear");
				printf("Invalid input please try again\n\n");
				break;
		}
	}
}