#include <string.h>
#include <ctype.h>
#include "firstPass.h"
#include "../first_pass/parseOperands.h"

/* Trim leading and trailing whitespace from a string in-place.
   Returns a pointer to the first non-space character. */
static char *trimSpaces(char *s)
{
    char *end;

    /* trim leading */
    while (*s && isspace((unsigned char)*s))
        s++;

    if (*s == '\0')
        return s;

    /* trim trailing */
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end))
        *end-- = '\0';

    return s;
}

int parseOperands(char *operandStr, char *op1, char *op2)
{
    char *token;
    char *trimmed;
    int count = 0;

    op1[0] = '\0';
    op2[0] = '\0';

    token = strtok(operandStr, ",");

    while (token != NULL && count < 2)
    {
        trimmed = trimSpaces(token);

        if (count == 0)
            strcpy(op1, trimmed);
        else
            strcpy(op2, trimmed);

        count++;
        token = strtok(NULL, ",");
    }

    return count;
}
