#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>

#define MAX_DATA_IMAGE 4096  /* total memory words available */
#define IC_START       100   /* instruction counter starts at 100 */




/* run the first pass on the .am file */
int runFirstPass(FILE *fp);




/* IC & DC counters */
void updateIC(int delta);
void updateDC(int delta);
int  getIC(void);
int  getDC(void);
void resetIC(void);
void resetDC(void);




/* return pointer to the shared data image array */
int *getDataImage(void);




/* split the operand string and return the number of operands found */
int parseOperands(char *operandStr, char *op1, char *op2);

#endif
