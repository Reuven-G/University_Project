#include <string.h>
#include "instructionTable.h"

extern Instruction instructionTable[];

Instruction *findInstruction(char *name)
{
    int i = 0;

    while (instructionTable[i].name != NULL)
    {
        if (strcmp(instructionTable[i].name, name) == 0)
            return &instructionTable[i];

        i++;
    }

    return NULL;
}
