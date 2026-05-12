#include <stdio.h>
#include <string.h>

#include "firstPass.h"
#include "../utils/utils.h"
#include "../analyzeRow/analyzeRow.h"
#include "../symbolTable/symbolTable.h"

#define MAX_LINE_LEN 82

/*
this script is the most center of the whole program.
*/




/* the func checks the symbol table and add the final IC to every DATA_LABEL type */
static void offsetDataSymbols(void)
{
    Symbol *sym  = head;
    int finalIC  = getIC();
 
    while (sym != NULL)
    {
        if (sym->type == DATA_LABEL)
        {
            sym->address += finalIC;
        }
        else if (sym->type == ENTRY_LABEL && sym->address < IC_START)
        {
            sym->address += finalIC;
        }
        sym = sym->next;
    }
}




/* the func search for ENTRY_LABEL symbols that their address is 0 and connects them the their real label */
static void resolveEntryLabels(void)
{
    Symbol *ent = head;
    Symbol *def;
 
    while (ent != NULL)
    {
        if (ent->type == ENTRY_LABEL && ent->address == 0)
        {
            def = head;
            while (def != NULL)
            {
                if (def != ent &&
                    strcmp(def->name, ent->name) == 0 &&
                    (def->type == CODE_LABEL || def->type == DATA_LABEL))
                {
                    ent->address = def->address;
                    break;
                }
                def = def->next;
            }
        }
        ent = ent->next;
    }
}




/* that func reads every and analayze it */
int runFirstPass(FILE *fp)
{
    char line[MAX_LINE_LEN];
    int  lineNum  = 0;
    int  hasError = 0;
 
    while (fgets(line, MAX_LINE_LEN, fp) != NULL)
    {
        lineNum++;
 
        if (isEmptyLine(line) || isComment(line))
            continue;
 
        if (!analyzeRow(line, lineNum))
            hasError = 1;
    }
 
    offsetDataSymbols();
    resolveEntryLabels();
 
    if (hasError)
    {
        fprintf(stderr, "First pass finished with errors.\n");
        return 0;
    }
    return 1;
}
