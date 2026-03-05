#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"

/* ראש הרשימה – גלובלי לקובץ הזה בלבד */
static Symbol *head = NULL;

/*
   מוסיף לייבל חדש לטבלת הסמלים.
   אם הלייבל כבר קיים – מדפיס שגיאה.
*/
void addSymbol(char *name, int address, SymbolType type)
{
    Symbol *newSymbol;
    Symbol *current;

    /* בדיקה אם כבר קיים */
    current = findSymbol(name);
    if (current != NULL)
    {
        printf("Error: symbol '%s' already exists\n", name);
        return;
    }

    /* הקצאת זיכרון לצומת חדש */
    newSymbol = (Symbol *)malloc(sizeof(Symbol));
    if (newSymbol == NULL)
    {
        printf("Error: memory allocation failed\n");
        exit(1);
    }

    strcpy(newSymbol->name, name);
    newSymbol->address = address;
    newSymbol->type = type;
    newSymbol->next = head;

    head = newSymbol;
}

/*
   מחפש לייבל בטבלה ומחזיר מצביע אליו.
   אם לא נמצא – מחזיר NULL.
*/
Symbol *findSymbol(char *name)
{
    Symbol *current = head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
            return current;

        current = current->next;
    }

    return NULL;
}

/*
   מדפיס את כל טבלת הסמלים.
   שימושי לבדיקה.
*/
void printSymbolTable(void)
{
    Symbol *current = head;

    printf("---- Symbol Table ----\n");

    while (current != NULL)
    {
        printf("Name: %s | Address: %d | Type: %d\n",
               current->name,
               current->address,
               current->type);

        current = current->next;
    }
}

/*
   משחרר את כל הזיכרון של הטבלה.
*/
void freeSymbolTable(void)
{
    Symbol *current = head;
    Symbol *temp;

    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }

    head = NULL;
}
