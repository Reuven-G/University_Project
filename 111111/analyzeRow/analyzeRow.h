#ifndef ANALYZEROW_H
#define ANALYZEROW_H

#include "../utils/utils.h"

#define MAX_LABEL_SIZE 31

typedef enum { COMMAND_LINE, DATA_LINE, EMPTY_LINE, ERROR_LINE } LineType;

typedef struct {
    LineType type;
    int hasLabel;
    char labelName[MAX_LABEL_SIZE];
    int isData;           /* 1 עבור הנחיות כמו .data או .string */
    int lValue;           /* כמה מילים השורה תופסת בזיכרון */
    int hasError;
    char errorMsg[100];
} LineInfo;

LineInfo analyzeRow(char *line);

#endif
