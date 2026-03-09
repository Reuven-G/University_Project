#ifndef INSTRUCTION_TABLE_H
#define INSTRUCTION_TABLE_H

typedef struct {

    char *name;
    int opcode;
    int operands;

} Instruction;

Instruction *findInstruction(char *name);

#endif
