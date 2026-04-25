#include "addressingType.h"
#include "labels.h"

int getAddressingType(char *operand)
{
    if (operand[0] == '#')
        return IMMEDIATE;

    if (operand[0] == '%')
        return RELATIVE;

    if (isRegisterName(operand))
        return REGISTER;

    return DIRECT;
}
