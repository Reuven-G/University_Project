#include "utils.h"




/* skip blank space */
int skipWhiteChars(const char *line, int i)
{
    while (line[i] == ' ' || line[i] == '\t')
    {
        i++;
    }

    return i;
}
