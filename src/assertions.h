#pragma once

#include "typedefs.h"

#if ASSERTIONS_ENABLED

#define debugBreak() __asm { int 3 }

#define ASSERT(expr) \
    if (expr) { \
    } else { \
        reportAssertionFailure(#expr, __FILE__, __LINE__); \
        debugBreak(); \
    } \

#else

#define ASSERT(expr)

#endif

void reportAssertionFailure(char* expr, char* fileName, S32 lineNumber);