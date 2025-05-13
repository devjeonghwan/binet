#include "binet/core/matrix.h"

#include "binet/core/matrix_internal.h"
#include "binet/api.h"
#include "binet/api_internal.h"

#include <stdio.h>
#include <stdlib.h>

BiStatus BiCreateMatrix(BiMatrix** matrix, const BiSize* shape, const BiRank rank, const BiDataType type)
{
    if (matrix == NULL || shape == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    if (rank < 1 || rank > BI_MAX_RANK)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_SHAPE);

    for (int i = 0; i < rank; i++)
        if (shape[i] < 1)
            BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_SHAPE);

    _BiMatrix* newMatrix = _BiAllocation(sizeof(_BiMatrix) + sizeof(BiSize) * rank + sizeof(BiSize) * rank);

    if (newMatrix == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);

    newMatrix->shape   = (BiSize*)(newMatrix + 1);
    newMatrix->strides = newMatrix->shape + rank;

    BiSize currentStride = 1;
    for (int i = rank - 1; i >= 0; i--)
    {
        newMatrix->shape[i]   = shape[i];
        newMatrix->strides[i] = currentStride;

        currentStride *= shape[i];
    }

    newMatrix->rank = rank;
    newMatrix->type = type;

    newMatrix->data = _BiAllocation(BiGetDataTypeSize(type, currentStride));

    if (newMatrix->data == NULL)
    {
        _BiDeallocation(newMatrix);
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);
    }

    *matrix = (BiMatrix*)newMatrix;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiDestroyMatrix(BiMatrix** matrix)
{
    if (matrix == NULL || *matrix == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    _BiMatrix* _matrix = (_BiMatrix*)*matrix;

    if (_matrix->data != NULL)
    {
        _BiDeallocation(_matrix->data);
        _matrix->data = NULL;
    }

    _BiDeallocation(_matrix);
    *matrix = NULL;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus _BiAppendToBuffer(char** buffer, size_t* capacity, size_t* length, const char* text)
{
    size_t textLength = 0;
    while (text[textLength] != '\0')
        textLength++;

    if (*length + textLength + 1 > *capacity)
    {
        const size_t newCapacity = (*capacity + textLength + 1) * 2;
        char*        newBuffer   = _BiAllocation(newCapacity);

        if (newBuffer == NULL)
            BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);

        for (size_t i    = 0; i < *length; i++)
            newBuffer[i] = (*buffer)[i];

        _BiDeallocation(*buffer);
        *buffer   = newBuffer;
        *capacity = newCapacity;
    }

    for (size_t i              = 0; i < textLength; i++)
        (*buffer)[*length + i] = text[i];

    *length += textLength;
    (*buffer)[*length] = '\0';

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus _BiSerializeMatrixString(char**           buffer, size_t* capacity, size_t* length,
                                  const _BiMatrix* matrix,
                                  const bool       pretty)
{
    const BiSize* shape = matrix->shape;
    const BiSize  rank  = matrix->rank;

    size_t indices[BI_MAX_RANK] = {0};
    size_t offsets[BI_MAX_RANK] = {0};

    size_t   depth  = 0;
    BiStatus status = _BiAppendToBuffer(buffer, capacity, length, "[");

    if (status != BI_STATUS_SUCCESS)
        return status;

    while (depth < rank)
    {
        if (indices[depth] < shape[depth])
        {
            const size_t nextOffset = offsets[depth] + indices[depth] * matrix->strides[depth];

            if (depth + 1 == rank)
            {
                char elementBuffer[64];

                switch (matrix->type)
                {
                case BI_DATA_TYPE_BINARY:
                    {
                        const BiUInt8* data      = matrix->data;
                        const size_t   byteIndex = nextOffset / 8;
                        const size_t   bitIndex  = nextOffset % 8;
                        const BiUInt8  bit       = (data[byteIndex] >> bitIndex) & 1;
                        snprintf(elementBuffer, sizeof(elementBuffer), "%u", bit);
                        break;
                    }
                case BI_DATA_TYPE_INT16:
                    {
                        const BiInt16* data = matrix->data;
                        snprintf(elementBuffer, sizeof(elementBuffer), "%d", data[nextOffset]);
                        break;
                    }
                case BI_DATA_TYPE_UINT16:
                    {
                        const BiUInt16* data = matrix->data;
                        snprintf(elementBuffer, sizeof(elementBuffer), "%d", data[nextOffset]);
                        break;
                    }
                case BI_DATA_TYPE_FLOAT32:
                    {
                        const BiFloat32* data = matrix->data;
                        snprintf(elementBuffer, sizeof(elementBuffer), "%.2f", data[nextOffset]);
                        break;
                    }
                default:
                    BI_RETURN_STATUS(BI_STATUS_ERROR_UNSUPPORTED_DATA_TYPE);
                }

                status = _BiAppendToBuffer(buffer, capacity, length, elementBuffer);
                if (status != BI_STATUS_SUCCESS)
                    return status;

                indices[depth]++;

                if (indices[depth] < shape[depth])
                {
                    status = _BiAppendToBuffer(buffer, capacity, length, pretty ? ",\n " : ", ");
                    if (status != BI_STATUS_SUCCESS)
                        return status;
                }
            }
            else
            {
                status = _BiAppendToBuffer(buffer, capacity, length, "[");
                if (status != BI_STATUS_SUCCESS)
                    return status;

                depth++;
                indices[depth] = 0;
                offsets[depth] = nextOffset;
            }
        }
        else
        {
            status = _BiAppendToBuffer(buffer, capacity, length, "]");
            if (status != BI_STATUS_SUCCESS)
                return status;

            if (depth == 0)
                break;

            depth--;
            indices[depth]++;

            if (indices[depth] < shape[depth])
            {
                status = _BiAppendToBuffer(buffer, capacity, length, pretty ? ",\n " : ", ");
                if (status != BI_STATUS_SUCCESS)
                    return status;
            }
        }
    }

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiGetMatrixString(BiString** out, const BiMatrix* matrix, const bool pretty)
{
    if (out == NULL || matrix == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiMatrix* _matrix = (_BiMatrix*)matrix;

    size_t capacity = 256;
    size_t length   = 0;

    char* buffer = _BiAllocation(capacity);
    if (buffer == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);

    buffer[0] = '\0';

    BiStatus status = _BiSerializeMatrixString(&buffer, &capacity, &length, _matrix, pretty);
    if (status != BI_STATUS_SUCCESS)
    {
        _BiDeallocation(buffer);
        return status;
    }

    status = BiCreateString(out, buffer);
    _BiDeallocation(buffer);
    return status;
}

BiStatus BiGetMatrixDataType(BiDataType* out, const BiMatrix* matrix)
{
    if (matrix == NULL || out == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiMatrix* _matrix = (_BiMatrix*)matrix;

    *out = _matrix->type;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiGetBroadcastShape(BiSize* out, const BiSize* shape1, const BiSize* shape2, BiRank rank)
{
    if (out == NULL || shape1 == NULL || shape2 == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    if (!_BiGetBroadcastShape(out, shape1, shape2, rank))
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_SHAPE);

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}
