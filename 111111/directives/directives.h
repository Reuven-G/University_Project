#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#define DIR_NONE 0
#define DIR_DATA 1
#define DIR_STRING 2
#define DIR_EXTERN 3
#define DIR_ENTRY 4

int getDirectiveType(char *word);

void handleData(char *line, int *DC, int dataImage[]);
void handleString(char *line, int *DC, int dataImage[]);
void handleExtern(char *name);
void handleEntry(char *name);

#endif
