#include "addressingType.h"

extern int isRegister(char *word);

int getAddressingType(char *operand)
{
    if(operand[0] == '#')
        return IMMEDIATE;

    if(isRegister(operand))
        return REGISTER;

    return DIRECT;
}
