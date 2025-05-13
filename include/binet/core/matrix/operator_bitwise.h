#ifndef OPERATOR_BITWISE_H
#define OPERATOR_BITWISE_H

#include "binet/api.h"
#include "binet/core/matrix.h"

DLLEXPORT BiStatus BiComputeMatrixBitwiseAnd(BiMatrix*       destination,
                                             const BiMatrix* operand1,
                                             const BiMatrix* operand2);

DLLEXPORT BiStatus BiComputeMatrixBitwiseOr(BiMatrix*       destination,
                                            const BiMatrix* operand1,
                                            const BiMatrix* operand2);

DLLEXPORT BiStatus BiComputeMatrixBitwiseXor(BiMatrix*       destination,
                                             const BiMatrix* operand1,
                                             const BiMatrix* operand2);

DLLEXPORT BiStatus BiComputeMatrixBitwiseNot(BiMatrix*       destination,
                                             const BiMatrix* operand);

#endif //OPERATOR_BITWISE_H
