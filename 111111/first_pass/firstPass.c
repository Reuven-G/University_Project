#include <stdio.h>
#include <string.h>

#include "firstPass.h"
#include "../utils/utils.h"
#include "../analyzeRow/analyzeRow.h"
#include "../symbolTable/symbolTable.h"

#define MAX_LINE_LEN 82

/* After the first pass is complete, every DATA_LABEL address must be
   shifted by the final IC value so data sits after code in memory. */
static void offsetDataSymbols(void)
{
    Symbol *sym = head;
    int finalIC  = getIC();

    while (sym != NULL)
    {
        if (sym->type == DATA_LABEL)
            sym->address += finalIC;
        sym = sym->next;
    }
}

/* Match any ENTRY_LABEL symbols that were forward-declared
   against labels that got defined later in the file */
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

int runFirstPass(FILE *fp)
{
    char line[MAX_LINE_LEN];
    int  lineNum  = 0;
    int  hasError = 0;

    while (fgets(line, MAX_LINE_LEN, fp) != NULL)
    {
        lineNum++;

        /* Skip blank lines and comment lines */
        if (isEmptyLine(line) || isComment(line))
            continue;

        if (!analyzeRow(line, lineNum))
            hasError = 1;
    }

    /* Offset all data-segment labels by the final IC */
    offsetDataSymbols();
    resolveEntryLabels();

    if (hasError)
    {
        fprintf(stderr, "First pass finished with errors.\n");
        return 0;
    }
    return 1;
}
