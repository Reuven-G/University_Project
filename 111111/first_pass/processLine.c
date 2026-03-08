#include <stdio.h>

#include "../analyzeRow/analyzeRow.h"

void updateIC(void);
void updateDC(void);

void processLine(char *line)
{
    analyzeRow(line);


    updateIC();
}
