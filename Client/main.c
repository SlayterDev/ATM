#include "globals.h"
#include "net.h"
#include "view.h"

int main(int argc, char const *argv[]) {
	if (argc < 2 || argc > 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		return 1;
	}

	system("clear");

	initNet(atoi(argv[1]));

	welcome();

	return 0;
}