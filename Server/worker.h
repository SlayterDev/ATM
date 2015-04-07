#ifndef WORKER_H
#define WORKER_H

#include "globals.h"

/*
 Called from the thread handler.

 Takes an int containing the socket descriptor for the client,
 and a char pointer to the message recieved from the client
*/
int processRequest(int sockfd, char *request);

#endif