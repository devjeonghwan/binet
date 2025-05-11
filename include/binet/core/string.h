#ifndef STRING_H
#define STRING_H

#include "binet/api.h"

typedef struct BiNetString BiNetString;

DLLEXPORT BiNetStatus BiNetCreateString(BiNetString** string, const char* source);

DLLEXPORT BiNetStatus BiNetDestroyString(BiNetString** string);

DLLEXPORT BiNetStatus BiNetConcatString(BiNetString** out,
                                        const BiNetString* string1,
                                        const BiNetString* string2);

DLLEXPORT BiNetStatus BiNetGetStringData(const char** out,
                                         const BiNetString* string);
#endif //STRING_H
