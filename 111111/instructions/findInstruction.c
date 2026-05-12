#include <string.h>
#include "instructionTable.h"

extern Instruction instructionTable[];




/* searche the global instruction table for a command by name */
Instruction *findInstruction(char *name)
{
    int i = 0;

	/* loop throught the table */
    while (instructionTable[i].name != NULL)
    {
		/* compare names */
        if (strcmp(instructionTable[i].name, name) == 0)
            return &instructionTable[i];

        i++;
    }

    return NULL;
}
