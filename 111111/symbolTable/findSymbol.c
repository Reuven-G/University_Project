#include <string.h>

#include "symbolTable.h"

Symbol *findSymbol(char *name)
{
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
