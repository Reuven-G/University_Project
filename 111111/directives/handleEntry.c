#include <stdio.h>
#include "directives.h"
#include "../symbolTable/symbolTable.h"

void handleEntry(char *name)
{
    Symbol *sym = findSymbol(name);

    if (sym != NULL)
    {
        /* Symbol already defined — mark it as entry */
        sym->type = ENTRY_LABEL;
    }
    else
    {
        /* Forward reference — add placeholder with address 0.
           addSymbol will update the address when the real
           definition is encountered later. */
        addSymbol(name, 0, ENTRY_LABEL);
    }
}

