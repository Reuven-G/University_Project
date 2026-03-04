#include <string.h>
#include "labels.h"

/* list of all possible instructions */
char *commands[] =
{
  "mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop",NULL
};

/* check if given commant is in the list */
int isCommandName(char *name)
{
    int i = 0;

    while (commands[i] != NULL)
    {
        if (strcmp(name, commands[i]) == 0)
            return 1;
        i++;
    }

    return 0;
}
