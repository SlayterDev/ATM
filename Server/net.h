#ifndef NET_H
#define NET_H

#include "globals.h"

pthread_t clients[MAX_CLIENTS];

void beginServer(int portnum);
int writeToClient(int sockfd, char *message);

#endif