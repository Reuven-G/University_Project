#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

/* label type */
typedef enum
{
    CODE_LABEL,
    DATA_LABEL,
    EXTERN_LABEL
} SymbolType;

/* structure of a node in the symbol table */
typedef struct Symbol
{
    char name[31];
    int address;
    SymbolType type;
    struct Symbol *next;
} Symbol;

/* head of the lsit */
extern Symbol *head;

/* add new label to the table */
void addSymbol(char *name, int address, SymbolType type);

/* search for a label in the table */
Symbol *findSymbol(char *name);

/* print for test */
void printSymbolTable(void);

/* free memory */
void freeSymbolTable(void);

#endif
