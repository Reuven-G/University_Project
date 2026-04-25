#include "directives.h"
#include "symbolTable.h"

void handleExtern(char *name)
{
    addSymbol(name, 0, EXTERN_LABEL);
}
