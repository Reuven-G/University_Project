#include "directives.h"
#include "../symbol_table/symbol_table.h"

void handleExtern(char *name)
{
    addSymbol(name,0,0,1);
}
