#include <string.h>
#include "symbolTable.h"



/* check for a lable in table and return a NULL if not found */
Symbol *findSymbol(char *name)
{
    Symbol *current = head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
            return current;

        current = current->next;
    }

    return NULL;
}
