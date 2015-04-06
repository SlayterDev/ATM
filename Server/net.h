#ifndef NET_H
#define NET_H

#include "globals.h"

void beginServer(int portnum);
int writeToClient(int sockfd, char *message);

#endif