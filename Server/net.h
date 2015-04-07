#ifndef NET_H
#define NET_H

#include "globals.h"

/*
 Start the main server logic of the program

 Takes the number of the port to bind the socket to
*/
void beginServer(int portnum);

/*
 Write a buffer to the specified client

 Takes the client socket descripton, and a char pointer to the
 message to send to the client
*/
int writeToClient(int sockfd, char *message);

#endif