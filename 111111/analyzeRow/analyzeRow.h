#ifndef ANALYZEROW_H
#define ANALYZEROW_H

#include "../utils/utils.h"
#include "../instructions/instructionTable.h"


/*
scans one line from the file, updates memory counters, and catches errors
returns 1 for success, 0 for failure
*/
int analyzeRow(char *line, int lineNum);

#endif
