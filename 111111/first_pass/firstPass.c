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
		/* push forword if its a data label */
        if (sym->type == DATA_LABEL)
        {
            sym->address += finalIC;
        }
		/* update entry labels that point to data */
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
		/* search for entry labels without address */
        if (ent->type == ENTRY_LABEL && ent->address == 0)
        {
            def = head;
            while (def != NULL)
            {
                /* found */
				if (def != ent && strcmp(def->name, ent->name) == 0 && (def->type == CODE_LABEL || def->type == DATA_LABEL))
                {
                    ent->address = def->address;
                    break;
                }
                def = def->next;
            }
        }
		/* move to next symbol */
        ent = ent->next;
    }
}




/* that func reads every and analayze it */
int runFirstPass(FILE *fp)
{
    char line[MAX_LINE_LEN];
    int  lineNum  = 0;
    int  hasError = 0;
 
	/* read the file line after line */
    while (fgets(line, MAX_LINE_LEN, fp) != NULL)
    {
        lineNum++;
 
		/* skip blank lines and commenets */
        if (isEmptyLine(line) || isComment(line))
            continue;
 
        if (!analyzeRow(line, lineNum))
            hasError = 1; /* there is an errot but we keep looking for more */
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
