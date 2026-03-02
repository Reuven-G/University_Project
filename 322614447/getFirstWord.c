#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Function to get first word in the line */
void get_first_word(char *line, char *dest) {
    int i = 0, j = 0;

    /* Ignore all white space in the beggining before the first word */
    while (line[i] != '\0' && isspace(line[i])) {
        i++;
    }

    /* Get all the chars before the next white space */
    while (line[i] != '\0' && !isspace(line[i])) {
        dest[j] = line[i];
        i++;
        j++;
    }
    dest[j] = '\0';
}
