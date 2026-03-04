#include <stdio.h>
#include <string.h>

#include "analyzeRow.h"
#include "../labels/labels.h"
#include "../utils/utils.h"

/* Function to analyze inputed command line */
void analyzeRow(char *line)
{
    char label[31];
    char command[31];

    int i = 0;
    int j = 0;

    /* Check if the analyzied line is a label */
    while (line[i] != '\0' && line[i] != ':' && line[i] != ' ')
    {
        label[j++] = line[i++];
    }

    /* label found */
    if (line[i] == ':')
    {
        label[j] = '\0';
        
        /* check if the label is legal */
        if (!isValidLabel(label))
        {
            printf("Error: label '%s' is not valid\n", label);
            return;
        }

        printf("Label: %s\n", label);
        i++;
    }

    while (line[i] == ' ')
        i++;

    i = skipWhiteChars(line, i);
    
    j = 0;

    /* check command */
    while (line[i] != '\0' && line[i] != ' ' && line[i] != '\n')
    {
        command[j++] = line[i++];
    }

    command[j] = '\0';

    printf("Command: %s\n", command);
}
