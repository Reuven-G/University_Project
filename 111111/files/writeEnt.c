#include <stdio.h>
#include <string.h>
#include "files.h"
#include "../symbolTable/symbolTable.h"

void writeEntFile(const char *baseName)
{
    char    filename[256];
    FILE   *fp = NULL;
    Symbol *sym;

    /* First pass: check if any entry symbols exist at all */
    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL)
            break;
        sym = sym->next;
    }

    if (sym == NULL)
        return; /* no entry symbols — do not create the file */

    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ent");

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

    /* Write all entry symbols */
    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL)
            fprintf(fp, "%s %d\n", sym->name, sym->address);
        sym = sym->next;
    }

    fclose(fp);
}
