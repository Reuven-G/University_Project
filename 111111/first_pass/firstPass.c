#include <stdio.h>
#include "firstPass.h"
#include "../utils/utils.h"

void runFirstPass(FILE *fp) {
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
}
