#include <ctype.h>
#include "utils.h"




/* check if it is an empty line */
int isEmptyLine(const char *line)
{
    while (*line)
	{
        if (!isspace(*line))
            return 0;
        line++;
    }
    return 1;
}
