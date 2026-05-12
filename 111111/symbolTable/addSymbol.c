#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"




/* create new entry in the symbol table and add a new label */
void addSymbol(char *name, int address, SymbolType type)
{
	/* variables */
    Symbol *newSymbol;
    Symbol *current;

	/* check if already exist */
    current = findSymbol(name);
    if (current != NULL)
    {
        if (current->type == ENTRY_LABEL &&
            (type == CODE_LABEL || type == DATA_LABEL))
        {
			/* update */
            current->address = address;
            return;
        }
		/* error */
        fprintf(stderr, "Error: symbol '%s' already defined\n", name);
        return;
    }

    newSymbol = (Symbol *)malloc(sizeof(Symbol));
    if (newSymbol == NULL)
    {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }

	/* build the structure of the new symbol */
    strcpy(newSymbol->name, name);
    newSymbol->address = address;
    newSymbol->type    = type;
    newSymbol->next    = head;
    head               = newSymbol;
}

