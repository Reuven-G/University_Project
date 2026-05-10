#include <stdio.h>
#include "directives.h"
#include "../symbolTable/symbolTable.h"

void handleEntry(char *name)
{
    Symbol *sym = findSymbol(name);

    if (sym != NULL)
    {
        sym->type = ENTRY_LABEL;
    }
}
