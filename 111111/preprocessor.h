#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

#define MAX_LINE_LENGTH  82   /* 80 chars + newline + null */
#define MAX_MACRO_LINES  100  /* max lines inside one macro body */
#define MAX_MACRO_NAME   31   /* max label length + null */

/* One macro: its name and the lines of its body */
typedef struct MacroNode
{
    char name[MAX_MACRO_NAME];
    char *lines[MAX_MACRO_LINES];
    int  lineCount;
    struct MacroNode *next;
} MacroNode;

/* Head of the macro linked list */
typedef struct
{
    MacroNode *head;
} MacroTable;

/* Create an empty macro table */
MacroTable *createMacroTable(void);

/* Add a new macro to the table */
void addMacro(MacroTable *table, const char *name);

/* Append a body line to the most recently added macro */
void addMacroLine(MacroTable *table, const char *line);

/* Find a macro by name, returns NULL if not found */
MacroNode *findMacro(MacroTable *table, const char *name);

/* Free all memory used by the macro table */
void freeMacroTable(MacroTable *table);

/* Main pre-assembler entry point:
   reads .as file, expands macros, writes .am file */
void runPreprocessor(FILE *asFile, FILE *amFile);

#endif
