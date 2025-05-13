#ifndef MATRIX_H
#define MATRIX_H

#include "binet/api.h"
#include "binet/core/data_type.h"
#include "binet/core/string.h"

#define BI_MAX_RANK 8

typedef struct BiMatrix BiMatrix;

typedef uint16_t BiRank;
typedef uint16_t BiSize;

DLLEXPORT BiStatus BiCreateMatrix(BiMatrix**    matrix,
                                  const BiSize* shape,
                                  BiRank        rank,
                                  BiDataType    type);

DLLEXPORT BiStatus BiDestroyMatrix(BiMatrix** matrix);

DLLEXPORT BiStatus BiGetMatrixString(BiString**      out,
                                     const BiMatrix* matrix,
                                     bool            pretty);

DLLEXPORT BiStatus BiGetMatrixDataType(BiDataType*     out,
                                       const BiMatrix* matrix);

DLLEXPORT BiStatus BiGetBroadcastShape(BiSize*       out,
                                       const BiSize* shape1,
                                       const BiSize* shape2,
                                       BiRank        rank);

#endif //MATRIX_H
