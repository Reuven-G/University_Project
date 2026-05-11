#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H




/* label type */
typedef enum
{
    CODE_LABEL,
    DATA_LABEL,
    EXTERN_LABEL,
    ENTRY_LABEL
} SymbolType;




/* structure of a node in the symbol table */
typedef struct Symbol
{
    char name[31];
    int address;
    SymbolType type;
    struct Symbol *next;
} Symbol;




/* head of the list */
extern Symbol *head;




/* create new entry in the symbol table and add a new label */
void addSymbol(char *name, int address, SymbolType type);




/* search and find specific name and returns the pointer of it */
Symbol *findSymbol(char *name);




/* check the linked list of symbols and print the name, memory address and type of each entry */
void printSymbolTable(void);




/* freeing the memory for each symbol node */
void freeSymbolTable(void);

#endif
