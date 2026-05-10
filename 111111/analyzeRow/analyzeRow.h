#ifndef ANALYZEROW_H
#define ANALYZEROW_H

#include "../utils/utils.h"
#include "../instructions/instructionTable.h"


/* Analyze one source line during the first pass.
   IC and DC are updated internally through their module functions.
   Returns 1 on success, 0 if a (non-fatal) error was found on the line. */
int analyzeRow(char *line, int lineNum);

#endif
