#include <string.h>
#include "labels.h"




/* all possible instructions */
char *commands[] =
{
  "mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop",NULL
};




/* check if the string is a command in the list */
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
