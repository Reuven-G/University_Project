#include "utils.h"




/* check if it is a comment */
int isComment(const char *line)
{
  while (*line == ' ' || *line == '\t')
    line++;
  return *line == ';';
    
}
