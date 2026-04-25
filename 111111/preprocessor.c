#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor.h"
#include "utils.h"

/* ── Macro table management ─────────────────────────────── */

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

/* ── Internal helpers ───────────────────────────────────── */

/* Returns 1 if the line starts with the keyword "mcro" */
static int isMacroStart(const char *line)
{
    return (strncmp(line, "mcro", 4) == 0 &&
            (line[4] == ' ' || line[4] == '\t'));
}

/* Returns 1 if the line is "mcroend" */
static int isMacroEnd(const char *line)
{
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    return (strncmp(line + i, "mcroend", 7) == 0);
}

/* Copy the macro name from a "mcro <name>" line into dest */
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

/* ── Main pre-assembler ──────────────────────────────────── */

void runPreprocessor(FILE *asFile, FILE *amFile)
{
    char line[MAX_LINE_LENGTH];
    char firstName[MAX_MACRO_NAME];
    MacroNode *macro;
    int insideMacro = 0;
    MacroTable *table = createMacroTable();

    while (fgets(line, MAX_LINE_LENGTH, asFile))
    {
        /* Skip leading whitespace for keyword checks only */
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

        /* Check if this line is a macro call */
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
