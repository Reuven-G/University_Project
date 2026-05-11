#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* func that get the first word in the line */
void get_first_word(char *line, char *dest) {
    int i = 0, j = 0;

    /* ignore all space before the first word */
    while (line[i] != '\0' && isspace(line[i])) {
        i++;
    }

    /* get all the chars before the next white-space */
    while (line[i] != '\0' && !isspace(line[i])) {
        dest[j] = line[i];
        i++;
        j++;
    }
    dest[j] = '\0';
}
