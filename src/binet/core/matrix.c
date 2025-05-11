#include "binet/core/matrix.h"
#include "binet/core/matrix_internal.h"
#include "binet/core/matrix_string_serialize.h"
#include "binet/api.h"
#include "binet/api_internal.h"

#include <stdlib.h>

BiNetStatus BiNetCreateMatrix(BiNetMatrix** matrix, const BiNetSize* shape, const BiNetRank rank, const BiNetDataType type)
{
    if (matrix == NULL || shape == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    if (rank < 1 || rank > 8)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_SHAPE);

    for (int i = 0; i < rank; i++)
        if (shape[i] < 1)
            BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_SHAPE);

    _BiNetMatrix* newMatrix = _BiNetAllocation(sizeof(_BiNetMatrix) + sizeof(BiNetSize) * rank + sizeof(BiNetSize) * rank);

    if (newMatrix == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);

    newMatrix->shape = (BiNetSize*)(newMatrix + 1);
    newMatrix->strides = newMatrix->shape + rank;

    BiNetSize currentStride = 1;
    for (int i = rank - 1; i >= 0; i--)
    {
        newMatrix->shape[i] = shape[i];
        newMatrix->strides[i] = currentStride;

        currentStride *= shape[i];
    }

    newMatrix->rank = rank;
    newMatrix->type = type;

    newMatrix->data = _BiNetAllocation(BiNetGetDataTypeSize(type, currentStride));

    if (newMatrix->data == NULL)
    {
        _BiNetDeallocation(newMatrix);
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);
    }

    *matrix = (BiNetMatrix*)newMatrix;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus BiNetDestroyMatrix(BiNetMatrix** matrix)
{
    if (matrix == NULL || *matrix == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    _BiNetMatrix* _matrix = (_BiNetMatrix*)*matrix;

    if (_matrix->data != NULL)
    {
        _BiNetDeallocation(_matrix->data);
        _matrix->data = NULL;
    }

    _BiNetDeallocation(_matrix);
    *matrix = NULL;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus BiNetGetMatrixString(BiNetString** out, const BiNetMatrix* matrix, bool pretty)
{
    if (out == NULL || matrix == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiNetMatrix* _matrix = (_BiNetMatrix*)matrix;

    size_t capacity = 256;
    size_t length = 0;

    char* buffer = _BiNetAllocation(capacity);
    if (buffer == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);

    buffer[0] = '\0';

    BiNetStatus status = _BiNetSerializeMatrixRecursive(&buffer, &capacity, &length, _matrix, 0, 0, pretty);
    if (status != BINET_STATUS_SUCCESS)
    {
        _BiNetDeallocation(buffer);
        return status;
    }

    status = BiNetCreateString(out, buffer);
    _BiNetDeallocation(buffer);
    return status;
}

BiNetStatus BiNetGetMatrixDataType(BiNetDataType* out, const BiNetMatrix* matrix)
{
    if (matrix == NULL || out == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiNetMatrix* _matrix = (_BiNetMatrix*)matrix;
    *out = _matrix->type;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus BiNetComputeBitwiseXor(BiNetMatrix** destination, const BiNetMatrix* operand1, const BiNetMatrix* operand2)
{
    if (operand1 == NULL || operand2 == NULL || destination == NULL)
    {
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);
    }

    // TODO Add *destination handling

    const _BiNetMatrix* _operand1 = (_BiNetMatrix*)operand1;
    const _BiNetMatrix* _operand2 = (_BiNetMatrix*)operand2;

    if (_operand1->rank != _operand2->rank)
    {
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_SHAPE);
    }

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}
