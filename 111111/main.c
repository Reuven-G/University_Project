#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessor.h"
#include "firstPass.h"
#include "symbolTable.h"

#define MAX_FILENAME 256

/* Build a filename by appending the given extension to the base name */
static void buildFilename(const char *base, const char *ext, char *out)
{
    strncpy(out, base, MAX_FILENAME - 5);
    out[MAX_FILENAME - 5] = '\0';
    strcat(out, ext);
}

/* Process one assembly source file through all three phases */
static void processFile(const char *baseName)
{
    char asName[MAX_FILENAME];
    char amName[MAX_FILENAME];
    FILE *asFile;
    FILE *amFile;

    buildFilename(baseName, ".as", asName);
    buildFilename(baseName, ".am", amName);

    /* ── Phase 0: Pre-assembler ── */
    asFile = fopen(asName, "r");
    if (asFile == NULL)
    {
        fprintf(stderr, "Error: cannot open file '%s'\n", asName);
        return;
    }

    amFile = fopen(amName, "w");
    if (amFile == NULL)
    {
        fprintf(stderr, "Error: cannot create file '%s'\n", amName);
        fclose(asFile);
        return;
    }

    runPreprocessor(asFile, amFile);
    fclose(asFile);
    fclose(amFile);

    /* ── Phase 1: First pass ── */
    amFile = fopen(amName, "r");
    if (amFile == NULL)
    {
        fprintf(stderr, "Error: cannot reopen file '%s'\n", amName);
        return;
    }

    resetIC();
    resetDC();
    freeSymbolTable();

    runFirstPass(amFile);
    fclose(amFile);

    /* ── Phase 2: Second pass ── (to be implemented) */
    /* runSecondPass(baseName); */
}

int main(int argc, char *argv[])
{
    int i;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: assembler <file1> [file2] ...\n");
        fprintf(stderr, "       (do not include the .as extension)\n");
        return 1;
    }

    for (i = 1; i < argc; i++)
    {
        printf("Processing: %s\n", argv[i]);
        processFile(argv[i]);
        freeSymbolTable();
    }

    return 0;
}
