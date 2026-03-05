#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"

/* add new label to the table after checking there arent one like this already */
void addSymbol(char *name, int address, SymbolType type)
{
    Symbol *newSymbol;

    /* check if there already a label like this */
    if (findSymbol(name) != NULL)
    {
        printf("Error: symbol '%s' already exists\n", name);
        return;
    }

    newSymbol = (Symbol *)malloc(sizeof(Symbol));
    /* check if the malloc function successeded */
    if (newSymbol == NULL)
    {
        printf("Error: memory allocation failed\n");
        exit(1);
    }

    copy the the info to the node */
    strcpy(newSymbol->name, name);
    newSymbol->address = address;
    newSymbol->type = type;

    /* make=ing the new node the head of the list */
    newSymbol->next = head;
    head = newSymbol;
}
