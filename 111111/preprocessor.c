#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "utils/utils.h"

/*
this script checks the .as file
*/




/* allocatee memory to the main structure that holds the list of the MACROS */
MacroTable *createMacroTable(void)
{
    MacroTable *table = (MacroTable *)malloc(sizeof(MacroTable));
    if (table == NULL)
    {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }
    table->head = NULL;
    return table;
}




/* the func recognizes new MACRO and and creates new node in the list with the MACRO name */
void addMacro(MacroTable *table, const char *name)
{
    MacroNode *node = (MacroNode *)malloc(sizeof(MacroNode));
    if (node == NULL)
    {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }
    strncpy(node->name, name, MAX_MACRO_NAME - 1);
    node->name[MAX_MACRO_NAME - 1] = '\0';
    node->lineCount = 0;
    node->next = table->head;
    table->head = node;
}





/* that func add one node of line of text in the size of the line to the array of the MACROS */
void addMacroLine(MacroTable *table, const char *line)
{
    MacroNode *node = table->head;
    if (node == NULL || node->lineCount >= MAX_MACRO_LINES)
        return;
 
    node->lines[node->lineCount] = (char *)malloc(strlen(line) + 1);
    if (node->lines[node->lineCount] == NULL)
    {
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(1);
    }
    strcpy(node->lines[node->lineCount], line);
    node->lineCount++;
}





/* the func searching for something suitble in the MACROS table */
MacroNode *findMacro(MacroTable *table, const char *name)
{
    MacroNode *current = table->head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}




/* that func frees the memory of all lines and the table */
void freeMacroTable(MacroTable *table)
{
    MacroNode *current = table->head;
    MacroNode *temp;
    int i;
 
    while (current != NULL)
    {
        for (i = 0; i < current->lineCount; i++)
            free(current->lines[i]);
        temp = current;
        current = current->next;
        free(temp);
    }
    free(table);
}




/* checks if the line starts with "mcro" and with/without tab space after it */
static int isMacroStart(const char *line)
{
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') i++;
    if (strncmp(line + i, "mcro", 4) != 0) return 0;
    i += 4;
    return (line[i] == ' ' || line[i] == '\t');
}




/* checks if the line ends with "mcroend"*/
static int isMacroEnd(const char *line)
{
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') i++;
    if (strncmp(line + i, "mcroend", 7) != 0) return 0;
    i += 7;
    return (line[i] == '\0' || line[i] == '\n' || line[i] == '\r' || line[i] == ' ' || line[i] == '\t');
}




/* extract and copy the macro name itself from the line */
static void extractMacroName(const char *line, char *dest)
{
    int i = 0;
    int j = 0;
 
    while (line[i] == ' ' || line[i] == '\t') i++; /* skip whitespace */
    
    i += 4; /* skip "mcro" */
    
    while (line[i] == ' ' || line[i] == '\t') i++; /* skip whitespace between mcro and name */
 
    while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r' && line[i] != ' '  && line[i] != '\t')
        dest[j++] = line[i++];
 
    dest[j] = '\0';
}




/* get the first word that is not a label */
static void getFirstNonLabelWord(const char *line, char *dest)
{
    char word[MAX_MACRO_NAME];
    int  i = 0;
    int  j;
 
    dest[0] = '\0';
 
    while (1)
    {
        /* skip whitespace */
        while (line[i] == ' ' || line[i] == '\t') i++;
        if (line[i] == '\0' || line[i] == '\n') break;
 
        /* read one word */
        j = 0;
        while (line[i] != '\0' && line[i] != ' ' &&
               line[i] != '\t' && line[i] != '\n')
            word[j++] = line[i++];
        word[j] = '\0';
 
        /* if ends with ':' skip and continue */
        if (j > 0 && word[j - 1] == ':')
            continue;
 
        /* found */
        strncpy(dest, word, MAX_MACRO_NAME - 1);
        dest[MAX_MACRO_NAME - 1] = '\0';
        break;
    }
}




/* that func evry line one after another and store the 'body' of the MACRO by checking when MACRO starts or ends */
void runPreprocessor(FILE *asFile, FILE *amFile)
{
    char       line[MAX_LINE_LENGTH];
    char       firstName[MAX_MACRO_NAME];
    MacroNode *macro;
    int        insideMacro = 0;
    MacroTable *table = createMacroTable();
    int        k;
 
    while (fgets(line, MAX_LINE_LENGTH, asFile))
    {
        if (isComment(line) || isEmptyLine(line))
        {
            if (!insideMacro)
                fputs(line, amFile);
            continue;
        }
 
        if (isMacroEnd(line))
        {
            insideMacro = 0;
            continue;
        }
 
        if (insideMacro)
        {
            addMacroLine(table, line);
            continue;
        }
 
        if (isMacroStart(line))
        {
            extractMacroName(line, firstName);
            addMacro(table, firstName);
            insideMacro = 1;
            continue;
        }
 
        /* check if the line have macro call */
        getFirstNonLabelWord(line, firstName);
        macro = findMacro(table, firstName);
 
        if (macro != NULL)
        {
            /* expand the macro: write all body lines to .am */
            for (k = 0; k < macro->lineCount; k++)
                fputs(macro->lines[k], amFile);
        }
        else
        {
            fputs(line, amFile);
        }
    }
 
    freeMacroTable(table);
}
