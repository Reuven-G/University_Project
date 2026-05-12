#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preprocessor.h"
#include "first_pass/firstPass.h"
#include "second_pass/secondPass.h"
#include "symbolTable/symbolTable.h"

#define MAX_FILENAME 256





/* The func adds extension to the filename */
static void buildFilename(const char *base, const char *ext, char *out)
{
    strncpy(out, base, MAX_FILENAME - 5);
    out[MAX_FILENAME - 5] = '\0';
    strcat(out, ext);
}





/* the func put the file throught a 'pipeline' of phases of the whole program */
static void processFile(const char *baseName)
{
    char asName[MAX_FILENAME];
    char amName[MAX_FILENAME];
    FILE *asFile;
    FILE *amFile;
    int   firstPassOk;

    buildFilename(baseName, ".as", asName);
    buildFilename(baseName, ".am", amName);



    /* preassembler phase */
    asFile = fopen(asName, "r"); /* open the .as file to read */
    if (asFile == NULL)
    {
        fprintf(stderr, "Error: cannot open '%s'\n", asName);
        return;
    }

    amFile = fopen(amName, "w"); /* create .am file to write inside */
    if (amFile == NULL)
    {
        fprintf(stderr, "Error: cannot create '%s'\n", amName);
        fclose(asFile);
        return;
    }

    runPreprocessor(asFile, amFile); /* the algoritm that 'translates' every macro and write the .am */
    fclose(asFile);
    fclose(amFile);



    /* first phase */
    amFile = fopen(amName, "r");
    if (amFile == NULL)
    {
        fprintf(stderr, "Error: cannot reopen '%s'\n", amName);
        return;
    }

	/* start a 'new list' after the previous runs */
    resetIC();
    resetDC();
    freeSymbolTable();

	/* stop the program from starting phase 2 if there any errors before */
    firstPassOk = runFirstPass(amFile);
    fclose(amFile);

    if (!firstPassOk)
    {
        fprintf(stderr, "Skipping second pass for '%s' due to errors.\n",
                baseName);
        freeSymbolTable();
        resetIC();
        resetDC();
        return;
    }



    /* second phase */
    runSecondPass(baseName);

    /* start a 'new list' after the last run */
    freeSymbolTable();
    resetIC();
    resetDC();
}





/* checks the arguments given in the command line */
int main(int argc, char *argv[])
{
    int i;

	/* check if the user didnt put file-names in th terminal */
    if (argc < 2)
    {
        fprintf(stderr, "Usage: assembler <file1> [file2] ...\n");
        fprintf(stderr, "       (without the .as extension)\n");
        return 1;
    }
	
	/* check all files */
    for (i = 1; i < argc; i++)
    {
        printf("----- Processing: %s -----\n", argv[i]);
        processFile(argv[i]); /* sends the file to the func at the top of this script */
    }

    return 0;
}
