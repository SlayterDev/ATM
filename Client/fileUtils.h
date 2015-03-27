#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "globals.h"

/*
 Load data for program. Specifcally, cash on hand and stamps in
 machine.

 Function will first check if the file exists and read it if so.
 Otherwise, we'll create a file and initialize it with default
 values.
*/
void loadData();

/*
 Save new data into file

 Call this when a change has been made to either cash on hand
 or stamps in the machine.
*/
void dataChanged();

#endif