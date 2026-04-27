#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>

/* Run the second pass over the expanded .am file.
   Encodes every instruction into machine words, resolves all label
   references, records external reference sites, then writes the
   three output files (.ob, .ent, .ext).

   baseName — the input filename WITHOUT extension (used to open
              the .am file and to name the three output files). */
void runSecondPass(const char *baseName);

#endif
