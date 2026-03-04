#include <string.h>
#include "labels.h"

/* check if the string is r0-r7 */
int isRegisterName(char *name)
{
    if (strlen(name) == 2 &&
        name[0] == 'r' &&
        name[1] >= '0' &&
        name[1] <= '7')
    {
        return 1;
    }

    return 0;
}
