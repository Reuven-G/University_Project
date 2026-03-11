#include <string.h>
#include <ctype.h>
#include "../utils/utils.h"
#include "parseOperands.h"

int parseOperands(char *line, char *op1, char *op2)
{
    char *token;
    int count = 0;

    token = strtok(line, ",");

    while (token != NULL)
    {
        token = skipWhiteChars(token);

        if (count == 0)
            strcpy(op1, token);

        else if (count == 1)
            strcpy(op2, token);

        count++;

        token = strtok(NULL, ",");
    }

    return count;
}
