#ifndef GLOBALS_H
#define GLOBALS_H

// Set this to 1 to show debug output for server info
#define DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

// Socket descriptor. Refers to the socket after creation.
int sockDesc;

// Server struct. Holds all connection info for the server.
struct sockaddr_in server;

#endif