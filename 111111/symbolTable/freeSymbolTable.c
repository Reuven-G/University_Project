#include <stdlib.h>

#include "symbolTable.h"




/* freeing the memory for each symbol node */
void freeSymbolTable(void)
{
    Symbol *current = head;
    Symbol *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }

    head = NULL;
}
