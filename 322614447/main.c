#include <stdio.h>
#include <stdlib.h>

#include "analyzeRow/analyzeRow.h"

/*
   פונקציית main של האסמבלר.
   כרגע:
   - פותחת קובץ קלט
   - קוראת שורה שורה
   - שולחת כל שורה ל-analyzeRow
*/
int main(int argc, char *argv[])
{
    FILE *fp;
    char line[256];

    /* בדיקה שקיבלנו שם קובץ */
    if (argc != 2)
    {
        printf("Error: missing input file name\n");
        return 1;
    }

    /* פתיחת קובץ האסמבלר */
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Error: cannot open file %s\n", argv[1]);
        return 1;
    }

    /* קריאת הקובץ שורה שורה */
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        analyzeRow(line);
    }

    /* סגירת הקובץ */
    fclose(fp);

    return 0;
}
