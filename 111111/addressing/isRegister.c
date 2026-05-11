#include <string.h>




/* the func checks if it is a register */
int isRegister(char *word)
{
    if(strlen(word) != 2)
        return 0;

    if(word[0] != 'r')
        return 0;

    if(word[1] >= '0' && word[1] <= '7')
        return 1;

    return 0;
}
