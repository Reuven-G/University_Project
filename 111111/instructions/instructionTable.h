#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

typedef struct {

    char *name;
    int opcode;
    int funct;
    int operands;

    int srcAllowed[4];
    int dstAllowed[4];

} Instruction;

Instruction *findInstruction(char *name);

#endif
