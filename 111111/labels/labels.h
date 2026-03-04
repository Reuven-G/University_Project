#ifndef LABELS_H
#define LABELS_H

/* check if the name of the label valid */
int isValidLabel(char *label);

/* check if the name is a register */
int isRegisterName(char *name);

/* check if the name is an instruction */
int isCommandName(char *name);

/* check if name the is a directive */
int isDirectiveName(char *name);

#endif
