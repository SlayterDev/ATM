#include "net.h"
#include "worker.h"

int writeToClient(int sockfd, char *message) {
	if (write(sockfd, message, strlen(message)) < 0) {
		fprintf(stderr, "ERROR writing to socket\n");
		int ret = 1;
		pthread_exit(&ret);
	}

	return 0;
}

void *serverLoop(void *sockfdPtr) {
	printf("Recieved Connection!\n");

	int sockfd = (intptr_t)sockfdPtr;

	int n, ret = 1;
	char buffer[256];

	while ((n = read(sockfd, buffer, 255)) > 0) {
		printf("Recieved: %s\n", buffer);
		processRequest(sockfd, buffer);
		//n = write(sockfd, "203", 3);

		if (n < 0) {
			fprintf(stderr, "ERROR writing to socket\n");
			pthread_exit(&ret);
		}

		memset(buffer, 0, 255);
	}

	printf("Connection closed\n");

	return NULL;
}

void beginServer(int portnum) {
	int sockfd, newsockfd, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		fprintf(stderr, "ERROR opening socket\n");
		exit(1);
	}

	memset((char *)&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnum);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR binding socket\n");
		exit(1);
	}

	printf("Server started...\nUse ^C to quit\n");

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	int clientNo = 0;
	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
		if (newsockfd < 0) {
			fprintf(stderr, "ERROR accepting connection\n");
			exit(1);
		}

		pthread_t thread;

		if (pthread_create(&thread, NULL, serverLoop, (void *)(intptr_t)newsockfd)) {
			fprintf(stderr, "ERROR creating thread\n");
			exit(1);
		}

		clientNo++;		
	}
}