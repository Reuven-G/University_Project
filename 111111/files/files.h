#ifndef FILES_H
#define FILES_H

/* External reference record: one entry per use site */
typedef struct
{
    char name[31];
    int  address;
} ExtRef;

/* Write the .ob object file.
   Header: "    IC DC"
   Then one line per word: "ADDR HEX ARE"
   codeImage/areImage: instruction words with their ARE letters.
   dataImage: data words (always ARE=A). */
void writeObFile(const char *baseName,
                 int *codeImage, char *areImage, int icSize,
                 int *dataImage, int dcSize);

/* Write the .ent file — only if ENTRY_LABEL symbols exist. */
void writeEntFile(const char *baseName);

/* Write the .ext file — only if extRefs count > 0. */
void writeExtFile(const char *baseName, ExtRef *extRefs, int count);

#endif

