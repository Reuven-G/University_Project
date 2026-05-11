#ifndef FILES_H
#define FILES_H




typedef struct
{
    char name[31];
    int  address;
} ExtRef;




/* create the .ob file */
void writeObFile(const char *baseName,
                 int *codeImage, char *areImage, int icSize,
                 int *dataImage, int dcSize);




/* create the .ent file */
void writeEntFile(const char *baseName);




/* create the .ext file */
void writeExtFile(const char *baseName, ExtRef *extRefs, int count);

#endif

