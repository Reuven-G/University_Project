#include <ctype.h>
#include "utils.h"

int isEmptyLine(const char *line) {
    while (*line) {
        if (!isspace(*line))
            return 0;
        line++;
    }
    return 1;
}
