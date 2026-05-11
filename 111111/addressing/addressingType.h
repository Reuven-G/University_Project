#ifndef ADDRESSING_TYPE_H
#define ADDRESSING_TYPE_H

#define IMMEDIATE 0
#define DIRECT    1
#define RELATIVE  2
#define REGISTER  3




/* checks the type of the of the operand */
int getAddressingType(char *operand);




/* the func checks if it is a register */
int isRegister(char *word)
#endif
