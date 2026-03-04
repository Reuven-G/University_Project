#include <string.h>
#include <ctype.h>
#include "labels.h"



/* check if the label is by the rules */
int isValidLabel(char *label)
{
    int i;
    /* check if first char is a letter */
    if (!isalpha(label[0]))
        return 0;
    /* check if more then 30 chars */
    if (strlen(label) > 30)
        return 0;

    for (i = 0; label[i] != '\0'; i++)
    {
        /* check if it has something that is not numbers or letters */
        if (!isalnum(label[i]))
            return 0;
    }
    /* check if register */
    if (isRegisterName(label))
        return 0;
    /* check if command */
    if (isCommandName(label))
        return 0;
    /* check if directive */
    if (isDirectiveName(label))
        return 0;

    return 1;
}
