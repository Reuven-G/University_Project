#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

void addSymbol(char *name, int address, SymbolType type)
{
    Symbol *newSymbol;
    Symbol *current;

    current = findSymbol(name);

    if (current != NULL)
    {
        /* Special case: forward .entry declaration being resolved
           by the real label definition — update address, keep ENTRY_LABEL */
        if (current->type == ENTRY_LABEL &&
            (type == CODE_LABEL || type == DATA_LABEL))
        {
            current->address = address;
            /* Keep type as ENTRY_LABEL so writeEnt finds it */
            return;
        }

        fprintf(stderr, "Error: symbol '%s' already defined\n", name);
        return;
    }

    newSymbol = (Symbol *)malloc(sizeof(Symbol));
    if (newSymbol == NULL)
    {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }

    strcpy(newSymbol->name, name);
    newSymbol->address = address;
    newSymbol->type    = type;
    newSymbol->next    = head;
    head               = newSymbol;
}
