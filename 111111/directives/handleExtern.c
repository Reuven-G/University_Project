#include "directives.h"
#include "../symbolTable/symbolTable.h"




/* add the label to the table and labels it as EXTERN */
void handleExtern(char *name)
{
    addSymbol(name, 0, EXTERN_LABEL);
}
