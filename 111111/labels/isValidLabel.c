#include <string.h>
#include <ctype.h>
#include "labels.h"



/* check if the label is legit */
int isValidLabel(char *label)
{
    int i;
    /* first char letter? */
    if (!isalpha(label[0]))
        return 0;
    /* more then 30 chars? */
    if (strlen(label) > 30)
        return 0;

    for (i = 0; label[i] != '\0'; i++)
    {
        /* it has something that is not numbers or letters? */
        if (!isalnum(label[i]))
            return 0;
    }
    /* register? */
    if (isRegisterName(label))
        return 0;
    /* command? */
    if (isCommandName(label))
        return 0;
    /* directive? */
    if (isDirectiveName(label))
        return 0;

    return 1;
}
