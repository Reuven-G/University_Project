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

/* head of the macro linked list */
typedef struct
{
    MacroNode *head;
} MacroTable;




/* allocatee memory to the main structure that holds the list of the MACROS */
MacroTable *createMacroTable(void);




/* the func recognizes new MACRO and and creates new node in the list with the MACRO name */
void addMacro(MacroTable *table, const char *name);




/* that func add one node of line of text in the size of the line to the array of the MACROS */
void addMacroLine(MacroTable *table, const char *line);




/* the func searching for something suitble in the MACROS table */
MacroNode *findMacro(MacroTable *table, const char *name);




/* that func frees the memory of all lines and the table */
void freeMacroTable(MacroTable *table);

/* that func evry line one after another and store the 'body' of the MACRO by checking when MACRO starts or ends */
void runPreprocessor(FILE *asFile, FILE *amFile);

#endif
