#include <string.h>
#include "labels.h"

/* all possible directives */
char *directives[] =
{
    ".data",".string",".entry",".extern",NULL
};

/* check if given directive is in the list */
int isDirectiveName(char *name)
{
    int i = 0;

    while (directives[i] != NULL)
    {
        if (strcmp(name, directives[i]) == 0)
            return 1;
        i++;
    }

    return 0;
}
