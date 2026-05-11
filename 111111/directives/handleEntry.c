#include <stdio.h>
#include "directives.h"
#include "../symbolTable/symbolTable.h"




/* updates the label as ENTRY or keeps a space for it for later use */
void handleEntry(char *name)
{
    Symbol *sym = findSymbol(name);

    if (sym != NULL)
    {
        /* mark the symbol as entry if already defined */
        sym->type = ENTRY_LABEL;
    }
    else
    {
        addSymbol(name, 0, ENTRY_LABEL);
    }
}

