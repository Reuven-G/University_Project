#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"

void addSymbol(char *name, int address, SymbolType type)
{
    Symbol *newSymbol;
    Symbol *current;

    /* check if symbol already exists */
    current = findSymbol(name);

    if (current != NULL)
    {
        printf("Error: symbol '%s' already exists\n", name);
        return;
    }

    /* allocate memory */
    newSymbol = (Symbol *)malloc(sizeof(Symbol));

    if (newSymbol == NULL)
    {
        printf("Error: memory allocation failed\n");
        exit(1);
    }

    strcpy(newSymbol->name, name);
    newSymbol->address = address;
    newSymbol->type = type;

    newSymbol->next = head;
    head = newSymbol;
}
