#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "utils/utils.h"

/*
	this script checks the .as file.
	inside it the script finds the MACROS, saves them, translate them
	and save everything in the .am file
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
    return (strncmp(line, "mcro", 4) == 0 &&
            (line[4] == ' ' || line[4] == '\t'));
}




/* checks if the line ends with "mcroend"*/
static int isMacroEnd(const char *line)
{
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    return (strncmp(line + i, "mcroend", 7) == 0);
}




/* extract and copy the macro name itself from the line */
static void extractMacroName(const char *line, char *dest)
{
    int i = 4; /* skip "mcro" */
    int j = 0;

    while (line[i] == ' ' || line[i] == '\t')
        i++;

    while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r' &&
           line[i] != ' '  && line[i] != '\t')
    {
        dest[j++] = line[i++];
    }
    dest[j] = '\0';
}




/* that func evry line one after another and store the 'body' of the MACRO by checking when MACRO starts or ends */
void runPreprocessor(FILE *asFile, FILE *amFile)
{
    char line[MAX_LINE_LENGTH];
    char firstName[MAX_MACRO_NAME];
    MacroNode *macro;
    int insideMacro = 0;
    MacroTable *table = createMacroTable();

    while (fgets(line, MAX_LINE_LENGTH, asFile))
    {
        /* skip white-space in the start */
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

        /* check if the line is a macro */
        get_first_word(line, firstName);
        macro = findMacro(table, firstName);

        if (macro != NULL)
        {
            int i;
            for (i = 0; i < macro->lineCount; i++)
                fputs(macro->lines[i], amFile);
        }
        else
        {
            fputs(line, amFile);
        }
    }

    freeMacroTable(table);
}
