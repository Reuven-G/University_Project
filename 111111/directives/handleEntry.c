#include <stdio.h>
#include "directives.h"
#include "symbolTable.h"

void handleEntry(char *name)
{
    Symbol *sym = findSymbol(name);

    if (sym == NULL)
    {
        /* Symbol not defined yet - add it now, address will be filled in second pass */
        addSymbol(name, 0, ENTRY_LABEL);
    }
    else
    {
        /* Symbol already exists from a label definition - mark it as entry */
        sym->type = ENTRY_LABEL;
    }
}
