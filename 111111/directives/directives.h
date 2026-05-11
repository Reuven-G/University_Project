#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#define DIR_NONE 0
#define DIR_DATA 1
#define DIR_STRING 2
#define DIR_EXTERN 3
#define DIR_ENTRY 4




/* checks and returns the type of the word */
int getDirectiveType(char *word);




/* tear down the input and check that the number is kosher */
void handleData(char *line, int *DC, int dataImage[]);




/* extract characters from inside quotas */
void handleString(char *line, int *DC, int dataImage[]);




/* add the label to the table and labels it as EXTERN */
void handleExtern(char *name);




/* updates the label as ENTRY or keeps a space for it for later use */
void handleEntry(char *name);

#endif
