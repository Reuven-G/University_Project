#ifndef ADDRESSING_TYPE_H
#define ADDRESSING_TYPE_H

#define IMMEDIATE 0
#define DIRECT    1
#define RELATIVE  2
#define REGISTER  3

int getAddressingType(char *operand);

#endif
