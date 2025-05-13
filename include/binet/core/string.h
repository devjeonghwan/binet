#ifndef STRING_H
#define STRING_H

#include "binet/api.h"

typedef struct BiString BiString;

DLLEXPORT BiStatus BiCreateString(BiString** string, const char* source);

DLLEXPORT BiStatus BiDestroyString(BiString** string);

DLLEXPORT BiStatus BiConcatString(BiString**      out,
                                  const BiString* string1,
                                  const BiString* string2);

DLLEXPORT BiStatus BiGetStringData(const char**    out,
                                   const BiString* string);
#endif //STRING_H
