#include <string.h>
#include <ctype.h>
#include "firstPass.h"
#include "../first_pass/parseOperands.h"

/*
that script works with the white-spaces before and after the line and checks the operands
*/
   



/* the func removes empty spaces */
static char *trimSpaces(char *s)
{
    char *end;

    /* remove from the start */
    while (*s && isspace((unsigned char)*s))
        s++;

    if (*s == '\0')
        return s;

    /* remove from the end */
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end))
        *end-- = '\0';

    return s;
}




/* the func checks the different opperands that were used */
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
		/* remove space around operands */
        trimmed = trimSpaces(token);
        
        if (trimmed[0] != '\0')
        {        
          if (count == 0)
              strcpy(op1, trimmed);
          else
              strcpy(op2, trimmed);
          count++;
        }
		
        token = strtok(NULL, ",");
    }

    return count;
}
