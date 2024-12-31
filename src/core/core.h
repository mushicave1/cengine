#ifndef CENGINE_CORE_CORE_H
#define CENGINE_CORE_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CENGINE_CORE_ASSERT(condition)                                             \
    do {                                                                    \
        if ( !(condition) ) {                                                 \
            fprintf(                                                        \
                stderr,                                                     \
                "Assertion Failed: %s\r\nFile: %s\r\nLine: %d\r\n",         \
                #condition,                                                 \
                __FILE__,                                                   \
                __LINE__                                                    \
            );                                                               \
        }                                                                   \
    } while (0)                                                             \

#define CENGINE_CORE_NULL_INDEX -1

const char* CENGINE_CORE_ReadFile(const char* file_path);

#endif
