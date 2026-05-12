#include <string.h>

#include "symbolTable.h"




/* search and find specific name and returns the pointer of it */
Symbol *findSymbol(char *name)
{
	/* variable */
    Symbol *current = head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}
