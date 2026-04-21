#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocessor/preprocessor.h"
#include "first_pass/firstPass.h"
#include "symbol_table/symbolTable.h"

/* המונים של הפרויקט - גלובליים כדי שכל הקבצים יראו אותם */
int IC = 100; /* פקודות מתחילות ב-100 */
int DC = 0;   /* נתונים מתחילים ב-0 */

/* מערכי הזיכרון של המחשב הדמיוני */
int codeImage[1200];
int dataImage[1200];

int main(int argc, char *argv[]) {
    int i;
    FILE *fp;

    if (argc < 2) {
        printf("Error: missing file name, niga!\n");
        return 1;
    }

    for (i = 1; i < argc; i++) {
        char fileName[256];
        /* בניית שם הקובץ עם סיומת as */
        sprintf(fileName, "%s.as", argv[i]);

        printf("--- Starting to work on: %s ---\n", fileName);

        fp = fopen(fileName, "r");
        if (fp == NULL) {
            printf("Error: can't open %s, check the path\n", fileName);
            continue;
        }

        /* שלב 1: מעבר ראשון - סריקת לייבלים וחישוב מרחקים (IC/DC) */
        runFirstPass(fp);

        /* הדפסה לבדיקה לראות שבאמת הכל נקלט בטבלה */
        printSymbolTable();
        
        printf("\nFinish First Pass. Status: IC=%d, DC=%d\n", IC, DC);

        fclose(fp);
        /* ניקוי טבלה לפני הקובץ הבא */
        freeSymbolTable(); 
        
        /* איפוס מונים לקובץ הבא */
        IC = 100;
        DC = 0;
    }

    return 0;
}
