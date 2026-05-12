#ifndef UTILS_H
#define UTILS_H




/* skip blank space */
int skipWhiteChars(const char *line, int i);




/* check if it is an empty line */
int isEmptyLine(const char *line);




/* check if it is a comment */
int isComment(const char *line);

#endif
