#include "fileUtils.h"

#define DATA_FILE "data.txt"

// Called when a file could not be opened
// The function will close the connection to the server and
// quit the application
void failure() {
	fprintf(stderr, "[-] Could not open data file\n");
	close(sockDesc);
	fprintf(stderr, "[+] Socket closed\n");
	exit(1);
}

void loadData() {
	// check if file exists
	if (access(DATA_FILE, F_OK) != -1) {
		// File exists
		FILE *f = fopen(DATA_FILE, "r");

		if (!f)
			failure();

		char buffer[100];

		// Get cash from file
		fgets(buffer, 100, f); 

		// strip new line
		buffer[strlen(buffer)-1] = '\0';

		char *tok = strtok(buffer, " ");
		tok = strtok(NULL, " ");
		cashInATM = atoi(tok);

		// Get stamps from file
		fgets(buffer, 100, f);

		// strip new line
		buffer[strlen(buffer)-1] = '\0';

		tok = strtok(buffer, " ");
		tok = strtok(NULL, " ");
		stampsInATM = atoi(tok);

		fclose(f);
	} else {
		// File does not exist
		FILE *f = fopen(DATA_FILE, "w");

		if (!f)
			failure();

		// Initialize default values
		cashInATM = MAX_CASH / 2; // Start with 1/2 of max cash
		fprintf(f, "Cash: %d\n", cashInATM);
		stampsInATM = DEFAULT_STAMPS;
		fprintf(f, "Stamps: %d\n", stampsInATM);

		fclose(f);
	}
}

void dataChanged() {
	FILE *f = fopen(DATA_FILE, "w");

	if (!f)
		failure();

	fprintf(f, "Cash: %d\n", cashInATM);
	fprintf(f, "Stamps: %d\n", stampsInATM);

	fclose(f);
}