#include <stdio.h>
#include "directives.h"
#include "../symbolTable/symbolTable.h"

void handleEntry(char *name)
{
    Symbol *sym = findSymbol(name);

    if (sym != NULL)
    {
        /* Already defined — mark it as entry */
        sym->type = ENTRY_LABEL;
    }
    else
    {
        /* Forward reference — add placeholder, address filled later */
        addSymbol(name, 0, ENTRY_LABEL);
    }
}
