#ifndef GLOBALS_H
#define GLOBALS_H

#define DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

int sockDesc;
struct sockaddr_in server;

#endif