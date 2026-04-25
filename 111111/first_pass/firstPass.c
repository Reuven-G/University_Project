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

void runFirstPass(FILE *fp)
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

    if (hasError)
        fprintf(stderr, "First pass finished with errors.\n");
}
