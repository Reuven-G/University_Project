#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "analyzeRow.h"
#include "../labels/labels.h"
#include "../utils/utils.h"
#include "../symbolTable/symbolTable.h"
#include "../directives/directives.h"
#include "../instructions/instructionTable.h"
#include "../addressing/addressingType.h"
#include "../first_pass/firstPass.h"
#include "../first_pass/parseOperands.h"
#include "../first_pass/calcInstructionSize.h"

#define MAX_LABEL_LEN  31
#define MAX_TOKEN_LEN  82

/*
that script take every line that given and analayzing it
*/




/* the func fishes every word in the line out between the blank spaces */
static int readToken(const char *line, int i, char *dest)
{
    int j = 0;
    i = skipWhiteChars(line, i);
    while (line[i] != '\0' && line[i] != ' ' &&
           line[i] != '\t' && line[i] != '\n')
    {
        dest[j++] = line[i++];
    }
    dest[j] = '\0';
    return i;
}




/* that func operates the instruction (does they exist in the table, what are they, etc.) */
static int handleInstruction(const char *cmdName, char *operandStr, int hasLabel, const char *labelName, int lineNum)
{
    char        op1[MAX_TOKEN_LEN];
    char        op2[MAX_TOKEN_LEN];
    char        op1copy[MAX_TOKEN_LEN];
    int         numOps;
    int         addrSrc = -1;
    int         addrDst = -1;
    int         L;
    Symbol     *existing;
    Instruction *inst;

    inst = findInstruction((char *)cmdName);
    if (inst == NULL)
    {
        fprintf(stderr, "Error line %d: unknown instruction '%s'\n",
                lineNum, cmdName);
        return 0;
    }

    strncpy(op1copy, operandStr, MAX_TOKEN_LEN - 1);
    op1copy[MAX_TOKEN_LEN - 1] = '\0';
    numOps = parseOperands(op1copy, op1, op2);

    if (numOps != inst->operands)
    {
        fprintf(stderr,
                "Error line %d: '%s' expects %d operand(s), got %d\n",
                lineNum, cmdName, inst->operands, numOps);
        return 0;
    }

    if (numOps == 2)
    {
        addrSrc = getAddressingType(op1);
        addrDst = getAddressingType(op2);

        if (!checkOperands(inst, addrSrc, addrDst))
        {
            fprintf(stderr,
                    "Error line %d: illegal addressing mode for '%s'\n",
                    lineNum, cmdName);
            return 0;
        }
    }
    else if (numOps == 1)
    {
        addrDst = getAddressingType(op1);

        if (!checkOperands(inst, -1, addrDst))
        {
            fprintf(stderr,
                    "Error line %d: illegal addressing mode for '%s'\n",
                    lineNum, cmdName);
            return 0;
        }
    }

    if (hasLabel)
    {
        existing = findSymbol((char *)labelName);
        if (existing != NULL && existing->type != ENTRY_LABEL)
        {
            fprintf(stderr,
                    "Error line %d: label '%s' already defined\n",
                    lineNum, labelName);
            return 0;
        }
        addSymbol((char *)labelName, getIC(), CODE_LABEL);
    }

    L = calcInstructionSize(numOps, addrSrc, addrDst);
    updateIC(L);

    return 1;
}




/* that func operates all the lines that start wit "." */
static int handleDirectiveLine(int dirType, char *rest, int hasLabel, const char *labelName, int lineNum)
{
    int     *dataImage = getDataImage();
    int      oldDC;
    int      dc;
    Symbol  *existing;

    switch (dirType)
    {
        case DIR_DATA:
            if (hasLabel)
            {
                existing = findSymbol((char *)labelName);
                if (existing != NULL && existing->type != ENTRY_LABEL)
                {
                    fprintf(stderr,
                            "Error line %d: label '%s' already defined\n",
                            lineNum, labelName);
                    return 0;
                }
                addSymbol((char *)labelName, getDC(), DATA_LABEL);
            }
            oldDC = getDC();
            dc    = oldDC;
            handleData(rest, &dc, dataImage);
            updateDC(dc - oldDC);
            break;

        case DIR_STRING:
            if (hasLabel)
            {
                existing = findSymbol((char *)labelName);
                if (existing != NULL && existing->type != ENTRY_LABEL)
                {
                    fprintf(stderr,
                            "Error line %d: label '%s' already defined\n",
                            lineNum, labelName);
                    return 0;
                }
                addSymbol((char *)labelName, getDC(), DATA_LABEL);
            }
            oldDC = getDC();
            dc    = oldDC;
            handleString(rest, &dc, dataImage);
            updateDC(dc - oldDC);
            break;

        case DIR_EXTERN:
        {
            char extName[MAX_LABEL_LEN];
            int i = 0;
            int j = 0;
            while (rest[i] == ' ' || rest[i] == '\t') i++;
            while (rest[i] && rest[i] != '\n' &&
                   rest[i] != ' ' && rest[i] != '\t')
                extName[j++] = rest[i++];
            extName[j] = '\0';

            if (!isValidLabel(extName))
            {
                fprintf(stderr,
                        "Error line %d: invalid extern label '%s'\n",
                        lineNum, extName);
                return 0;
            }
            handleExtern(extName);
            break;
        }

        case DIR_ENTRY:
        {
            char entName[MAX_LABEL_LEN];
            int i = 0;
            int j = 0;
            while (rest[i] == ' ' || rest[i] == '\t') i++;
            while (rest[i] && rest[i] != '\n' &&
                   rest[i] != ' ' && rest[i] != '\t')
                entName[j++] = rest[i++];
            entName[j] = '\0';

            if (!isValidLabel(entName))
            {
                fprintf(stderr,
                        "Error line %d: invalid entry label '%s'\n",
                        lineNum, entName);
                return 0;
            }
            handleEntry(entName);
            break;
        }
		
        default:
            fprintf(stderr, "Error line %d: unknown directive\n", lineNum);
            return 0;
    }
    return 1;
}




/* that func decides to witch class every line is belongs */
int analyzeRow(char *line, int lineNum)
{
    char label[MAX_LABEL_LEN];
    char command[MAX_TOKEN_LEN];
    int  hasLabel = 0;
    int  i = 0;
    int  j = 0;
    int  dirType;

    /* detect optional label (the word ends with ':') */
    i = skipWhiteChars(line, 0);
    j = 0;
    while (line[i] != '\0' && line[i] != ':' &&
           line[i] != ' '  && line[i] != '\t' && line[i] != '\n')
    {
        if (j < MAX_LABEL_LEN - 1)
            label[j++] = line[i];
        i++;
    }
    label[j] = '\0';

    if (line[i] == ':')
    {
        if (!isValidLabel(label))
        {
            fprintf(stderr,
                    "Error line %d: invalid label name '%s'\n",
                    lineNum, label);
            return 0;
        }
        hasLabel = 1;
        i++;
    }
    else
    {
        i = skipWhiteChars(line, 0);
    }

    i = readToken(line, i, command);

    if (command[0] == '\0')
        return 1;

    if (hasLabel &&
        (strcmp(command, ".entry") == 0 || strcmp(command, ".extern") == 0))
    {
        fprintf(stderr,
                "Warning line %d: label before '%s' is ignored\n",
                lineNum, command);
        hasLabel = 0;
    }

    i = skipWhiteChars(line, i);

    dirType = getDirectiveType(command);

    if (dirType != DIR_NONE)
    {
        return handleDirectiveLine(dirType, line + i,
                                   hasLabel, label, lineNum);
    }

    if (isCommandName(command))
    {
        return handleInstruction(command, line + i,
                                 hasLabel, label, lineNum);
    }

    fprintf(stderr,
            "Error line %d: unrecognized command '%s'\n",
            lineNum, command);
    return 0;
}

