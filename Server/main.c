#include "globals.h"
#include "net.h"

int main(int argc, char const *argv[]) {
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: %s <port> (host)\n", argv[0]);
		fprintf(stderr, "port: port number of the server\n");
		fprintf(stderr, "host: address of server. Uses \"127.0.0.1\" by default.\n");
		return 1;
	}

	beginServer(atoi(argv[1]));

	return 0;
}