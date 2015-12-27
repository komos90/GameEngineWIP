#include <cstdio>

#include "assertions.h"

void reportAssertionFailure(char* expr, char* fileName, S32 lineNumber) {
    printf("Assertion \"%s\" failed in file \"%s\" at line %d", expr, fileName, lineNumber);
}