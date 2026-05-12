#include <stdio.h>
#include <string.h>
#include "files.h"
#include "../symbolTable/symbolTable.h"




/* create the .ent file */
void writeEntFile(const char *baseName)
{
    char    filename[256];
    FILE   *fp;
    Symbol *sym;

    /* check if entry symbols exists */
    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL)
            break;
        sym = sym->next;
    }

	/* dont create the file if there no entry symbols */
    if (sym == NULL)
        return;

	/* create the file */
    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ent");

	/* allow writing to the file */
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

	/* write every entry symbol from the table to the file */
    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL)
            fprintf(fp, "%s %04d\n", sym->name, sym->address);
        sym = sym->next;
    }

    fclose(fp);
}

