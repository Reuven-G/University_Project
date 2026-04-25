#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>

#define MAX_DATA_IMAGE 4096  /* total memory words available */
#define IC_START       100   /* instruction counter starts at 100 */

/* ── Main entry point ─────────────────────────────────── */

/* Run the first pass over the expanded .am file.
   Fills the symbol table and data image, advances IC and DC. */
void runFirstPass(FILE *fp);

/* ── IC / DC counters ─────────────────────────────────── */

void updateIC(int delta);
void updateDC(int delta);
int  getIC(void);
int  getDC(void);
void resetIC(void);
void resetDC(void);

/* ── Data image ───────────────────────────────────────── */

/* Returns a pointer to the shared data image array */
int *getDataImage(void);

/* ── Operand parsing ──────────────────────────────────── */

/* Splits the operand string (after the command) by comma.
   Returns the number of operands found (0, 1, or 2).
   op1 and op2 must be buffers of at least 32 bytes. */
int parseOperands(char *operandStr, char *op1, char *op2);

#endif
