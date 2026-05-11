#ifndef LABELS_H
#define LABELS_H

/* check if the label is legit */
int isValidLabel(char *label);

/* check if the string is r0-r7 */
int isRegisterName(char *name);

/* check if the command in the list */
int isCommandName(char *name);

/* check if directive in the list */
int isDirectiveName(char *name);

#endif
