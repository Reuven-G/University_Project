#include <string.h>
#include "directives.h"

int getDirectiveType(char *word)
{
    if(strcmp(word,".data") == 0)
        return DIR_DATA;

    if(strcmp(word,".string") == 0)
        return DIR_STRING;

    if(strcmp(word,".extern") == 0)
        return DIR_EXTERN;

    if(strcmp(word,".entry") == 0)
        return DIR_ENTRY;

    return DIR_NONE;
}
