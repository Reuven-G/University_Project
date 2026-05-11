#include <stdio.h>
#include <string.h>
#include "files.h"
#include "../symbolTable/symbolTable.h"

void writeEntFile(const char *baseName)
{
    char    filename[256];
    FILE   *fp = NULL;
    Symbol *sym;

    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL)
            break;
        sym = sym->next;
    }

    if (sym == NULL)
        return;

    strncpy(filename, baseName, 251);
    filename[251] = '\0';
    strcat(filename, ".ent");

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", filename);
        return;
    }

    sym = head;
    while (sym != NULL)
    {
        if (sym->type == ENTRY_LABEL)
            fprintf(fp, "%s %04d\n", sym->name, sym->address);
        sym = sym->next;
    }

    fclose(fp);
}
