#ifndef FILES_H
#define FILES_H

/* ── Output file writers ───────────────────────────────────────────
   All three functions receive the base filename (without extension).
   They open/create the appropriate file themselves.
   writeEnt and writeExt do nothing and create no file if there are
   no relevant symbols to write.
─────────────────────────────────────────────────────────────────── */

/* Write the .ob object file:
     - Header line: "  IC  DC" (instruction count, data count)
     - One line per word: "ADDR  HEX_VALUE"
   codeImage[0..icSize-1] holds instruction words (addresses IC_START..IC_START+icSize-1).
   dataImage[0..dcSize-1] holds data words (addresses IC_START+icSize..). */
void writeObFile(const char *baseName,
                 int *codeImage, int icSize,
                 int *dataImage, int dcSize);

/* Write the .ent file: one "LABEL  ADDRESS" line per ENTRY_LABEL symbol.
   File is not created if there are no entry symbols. */
void writeEntFile(const char *baseName);

/* Write the .ext file: one "LABEL  ADDRESS" line per external reference.
   File is not created if there are no extern uses.
   extRefs is an array of (name, address) pairs, count is its length. */
typedef struct
{
    char name[31];
    int  address;
} ExtRef;

void writeExtFile(const char *baseName, ExtRef *extRefs, int count);

#endif
