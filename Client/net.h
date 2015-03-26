#ifndef NET_H
#define NET_H

#include "globals.h"

// Initialize the connection to the server
void initNet(int portNum);

/* 
 Send a message to the server

 Use this if the response from the server is expected to be only
 a response code. The function will return the response code sent
 by the server as an integer

 sendMessage assumes a null terminated string
 */
int sendMessage(char *message);

/*
 Send a message to the server

 Use this if the response from the server is expected to include
 more than just the response code. The function will return a
 pointer containing the message from the server. The caller is 
 responsible for tokenizing the buffer, and freeing the pointer

 sendMessageWithResponse assumes a null terminated string
*/
char *sendMessageWithResponse(char *message);

#endif