#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "directives.h"

#define WORD_MAX  2047   /* 2^11 - 1  (12-bit signed max) */
#define WORD_MIN -2048   /* -2^11     (12-bit signed min) */

/* Returns 1 if the string is a valid integer (optional leading +/-) */
static int isInteger(const char *s)
{
    if (*s == '+' || *s == '-')
        s++;
    if (*s == '\0')
        return 0;
    while (*s)
    {
        if (!isdigit((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}

/* Trim leading and trailing whitespace, return pointer into s */
static char *trim(char *s)
{
    char *end;
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

void handleData(char *line, int *DC, int dataImage[])
{
    char *token;
    char *trimmed;
    int   value;

    token = strtok(line, ",");

    while (token != NULL)
    {
        trimmed = trim(token);

        if (!isInteger(trimmed))
        {
            fprintf(stderr, "Error: '.data' — '%s' is not a valid integer\n",
                    trimmed);
            token = strtok(NULL, ",");
            continue;
        }

        value = atoi(trimmed);

        if (value < WORD_MIN || value > WORD_MAX)
        {
            fprintf(stderr,
                    "Error: '.data' — value %d is out of 12-bit range\n",
                    value);
            token = strtok(NULL, ",");
            continue;
        }

        dataImage[*DC] = value;
        (*DC)++;

        token = strtok(NULL, ",");
    }
}
