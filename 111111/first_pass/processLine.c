#include "../analyzeRow/analyzeRow.h"

/* processLine is a thin wrapper kept for structural compatibility.
   All real logic lives in analyzeRow. */
int processLine(char *line, int lineNum)
{
    return analyzeRow(line, lineNum);
}
