#include <stdio.h>
#include <string.h>
#include "analyzeRow.h"
#include "../labels/labels.h"
#include "../utils/utils.h"
#include "../instructions/instructionTable.h"
#include "../directives/directives.h"
#include "../first_pass/parseOperands.h"

/* מושך את המונים מה-main */
extern int IC;
extern int DC;

void analyzeRow(char *line)
{
    char label[31], command[31], op1[31], op2[31];
    int i = 0, j = 0, numOps = 0;
    Instruction *inst;

    /* 1. דילוג על שטויות (רווחים, הערות, שורות ריקות) */
    i = skipWhiteChars(line, i);
    if (line[i] == '\0' || line[i] == '\n' || line[i] == ';') {
        return; 
    }

    /* 2. ניסיון לחלץ מילה ראשונה (אולי זה לייבל?) */
    while (line[i] != '\0' && line[i] != ':' && line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
        label[j++] = line[i++];
    }
    label[j] = '\0';

    /* 3. בדיקה אם המילה היא לייבל (נגמרת ב-:) */
    if (line[i] == ':') {
        if (isValidLabel(label)) {
            /* שומרים את הלייבל עם הכתובת הנוכחית של IC */
            addSymbol(label, IC, CODE_LABEL); 
        } else {
            printf("Error: label %s is trash\n", label);
        }
        i++; /* מדלגים על ה-':' */
    } else {
        /* אם זה לא לייבל, המילה שקראנו היא כנראה הפקודה עצמה */
        i -= j; 
    }

    /* 4. דילוג לרווח הבא כדי למצוא את שם הפקודה/הנחיה */
    i = skipWhiteChars(line, i);
    j = 0;
    while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
        command[j++] = line[i++];
    }
    command[j] = '\0';

    if (j == 0) return;

    /* 5. טיפול בפקודות (Instructions - mov, add, etc) */
    inst = findInstruction(command);
    if (inst != NULL) {
        int L = 1; /* מילה בסיסית תמיד יש */
        
        /* מפרקים את האופרנדים (נמצא ב-parseOperands.c שלך) */
        numOps = parseOperands(line + i, op1, op2);

        /* חישוב אורך הפקודה (L) */
        if (numOps > 0) {
            /* קומבינה של רגיסטרים: שניהם נדחפים למילה אחת */
            if (numOps == 2 && isRegisterName(op1) && isRegisterName(op2)) {
                L += 1;
            } else {
                /* כל אופרנד אחר מקבל מילה משלו בזיכרון */
                L += numOps;
            }
        }
        
        IC += L; /* מקדמים את המונה לפי האורך שחישבנו */
        return;
    }

    /* 6. טיפול בהנחיות (Directives - .data, .string) */
    if (isDirectiveName(command)) {
        int dirType = getDirectiveType(command);
        
        if (dirType == DIR_DATA) {
            /* פה צריך להוסיף ל-DC לפי כמות המספרים */
            /* DC += countNumbers(line + i); */
        } else if (dirType == DIR_STRING) {
            /* פה צריך להוסיף ל-DC לפי אורך הסטרינג + 1 */
            /* DC += countStringChars(line + i); */
        }
    }
}
