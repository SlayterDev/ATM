#include "globals.h"
#include "net.h"
#include "data.h"

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		fprintf(stderr, "port: port number of the server\n");
		return 1;
	}

	readUsers();

	beginServer(atoi(argv[1]));

	return 0;
}