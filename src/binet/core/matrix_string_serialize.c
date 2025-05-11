#include "binet/core/matrix_string_serialize.h"
#include "binet/api_internal.h"

#include <stdio.h>

BiNetStatus _BiNetAppendToBuffer(char** buffer, size_t* capacity, size_t* length, const char* text)
{
    size_t textLength = 0;
    while (text[textLength] != '\0')
        textLength++;

    if (*length + textLength + 1 > *capacity)
    {
        const size_t newCapacity = (*capacity + textLength + 1) * 2;
        char* newBuffer = _BiNetAllocation(newCapacity);

        if (newBuffer == NULL)
            BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);

        for (size_t i = 0; i < *length; i++)
            newBuffer[i] = (*buffer)[i];

        _BiNetDeallocation(*buffer);
        *buffer = newBuffer;
        *capacity = newCapacity;
    }

    for (size_t i = 0; i < textLength; i++)
        (*buffer)[*length + i] = text[i];

    *length += textLength;
    (*buffer)[*length] = '\0';

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus _BiNetSerializeMatrixRecursive(char** buffer, size_t* capacity, size_t* length,
                                           const _BiNetMatrix* matrix, const size_t dimension, const size_t offset,
                                           const bool pretty)
{
    const BiNetSize* shape = matrix->shape;

    BiNetStatus status = _BiNetAppendToBuffer(buffer, capacity, length, "[");
    if (status != BINET_STATUS_SUCCESS) return status;

    for (size_t i = 0; i < shape[dimension]; i++)
    {
        const size_t nextOffset = offset + i * matrix->strides[dimension];

        if (dimension + 1 == matrix->rank)
        {
            char elementBuffer[64];
            switch (matrix->type)
            {
            case BINET_DATA_TYPE_BINARY:
                {
                    const uint8_t* data = matrix->data;
                    const size_t byteIndex = nextOffset / 8;
                    const size_t bitIndex = nextOffset % 8;
                    const uint8_t bit = (data[byteIndex] >> bitIndex) & 1;
                    snprintf(elementBuffer, sizeof(elementBuffer), "%u", bit);
                    break;
                }
            case BINET_DATA_TYPE_INTEGER16:
                {
                    const int16_t* data = matrix->data;
                    snprintf(elementBuffer, sizeof(elementBuffer), "%d", data[nextOffset]);
                    break;
                }
            case BINET_DATA_TYPE_FLOAT32:
                {
                    const float* data = matrix->data;
                    snprintf(elementBuffer, sizeof(elementBuffer), "%.2f", data[nextOffset]);
                    break;
                }
            default:
                BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);
            }

            status = _BiNetAppendToBuffer(buffer, capacity, length, elementBuffer);
            if (status != BINET_STATUS_SUCCESS)
                return status;
        }
        else
        {
            status = _BiNetSerializeMatrixRecursive(buffer, capacity, length, matrix, dimension + 1, nextOffset, pretty);
            if (status != BINET_STATUS_SUCCESS)
                return status;
        }

        if (i != shape[dimension] - 1)
        {
            status = _BiNetAppendToBuffer(buffer, capacity, length, pretty ? ",\n " : ", ");
            if (status != BINET_STATUS_SUCCESS)
                return status;
        }
    }

    return _BiNetAppendToBuffer(buffer, capacity, length, "]");
}
