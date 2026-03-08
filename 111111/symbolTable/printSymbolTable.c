#include <stdio.h>

#include "symbolTable.h"

void printSymbolTable(void)
{
    Symbol *current = head;

    printf("---- Symbol Table ----\n");

    while (current != NULL)
    {
        printf("Name: %s | Address: %d | Type: %d\n",
               current->name,
               current->address,
               current->type);

        current = current->next;
    }
}
