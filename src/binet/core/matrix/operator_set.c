#include "binet/core/matrix.h"
#include "binet/core/matrix_internal.h"
#include "binet/core/matrix/operator_set.h"
#include "binet/api_internal.h"

BiStatus _BiSetMatrixValuesIterative(const _BiMatrix* matrix, const void* values)
{
    const BiSize* shape = matrix->shape;
    const BiRank  rank  = matrix->rank;

    size_t   indices[BI_MAX_RANK] = {0};
    BiOffset offsets[BI_MAX_RANK] = {0};

    size_t depth      = 0;
    size_t valueIndex = 0;

    while (depth < rank)
    {
        if (indices[depth] < shape[depth])
        {
            const BiOffset nextOffset = offsets[depth] + (BiOffset)indices[depth] * matrix->strides[depth];

            if (depth + 1 == rank)
            {
                switch (matrix->type)
                {
                case BI_DATA_TYPE_BINARY:
                    {
                        const BiInt8* source      = values;
                        BiInt8*       destination = matrix->data;
                        const size_t  byteIndex   = nextOffset / 8;
                        const size_t  bitIndex    = nextOffset % 8;
                        const BiUInt8 bit         = source[valueIndex] & 1;

                        if (bit)
                            destination[byteIndex] |= (1 << bitIndex);
                        else
                            destination[byteIndex] &= ~(1 << bitIndex);
                        break;
                    }
                case BI_DATA_TYPE_INT16:
                    {
                        const BiInt16* source      = values;
                        BiInt16*       destination = matrix->data;
                        destination[nextOffset]    = source[valueIndex];
                        break;
                    }
                case BI_DATA_TYPE_UINT16:
                    {
                        const BiUInt16* source      = values;
                        BiUInt16*       destination = matrix->data;
                        destination[nextOffset]     = source[valueIndex];
                        break;
                    }
                case BI_DATA_TYPE_FLOAT32:
                    {
                        const BiFloat32* source      = values;
                        BiFloat32*       destination = matrix->data;
                        destination[nextOffset]      = source[valueIndex];
                        break;
                    }
                default:
                    BI_RETURN_STATUS(BI_STATUS_ERROR_UNSUPPORTED_DATA_TYPE);
                }

                valueIndex++;
                indices[depth]++;
            }
            else
            {
                depth++;
                indices[depth] = 0;
                offsets[depth] = nextOffset;
            }
        }
        else
        {
            if (depth == 0)
                break;

            depth--;
            indices[depth]++;
        }
    }

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiSetMatrixBinaryValues(BiMatrix* destination, const BiBool* values)
{
    if (destination == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiMatrix* _destination = (_BiMatrix*)destination;

    if (_destination->type != BI_DATA_TYPE_BINARY)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_DATA_TYPE);

    const BiStatus status = _BiSetMatrixValuesIterative(_destination, values);

    if (status != BI_STATUS_SUCCESS)
        return status;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiSetMatrixInt16Values(BiMatrix* destination, const BiInt16* values)
{
    if (destination == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiMatrix* _destination = (_BiMatrix*)destination;

    if (_destination->type != BI_DATA_TYPE_INT16)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_DATA_TYPE);

    const BiStatus status = _BiSetMatrixValuesIterative(_destination, values);

    if (status != BI_STATUS_SUCCESS)
        return status;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiSetMatrixUInt16Values(BiMatrix* destination, const BiUInt16* values)
{
    if (destination == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiMatrix* _destination = (_BiMatrix*)destination;

    if (_destination->type != BI_DATA_TYPE_UINT16)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_DATA_TYPE);

    const BiStatus status = _BiSetMatrixValuesIterative(_destination, values);

    if (status != BI_STATUS_SUCCESS)
        return status;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiSetMatrixFloat32Values(BiMatrix* destination, const BiFloat32* values)
{
    if (destination == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiMatrix* _destination = (_BiMatrix*)destination;

    if (_destination->type != BI_DATA_TYPE_FLOAT32)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_DATA_TYPE);

    const BiStatus status = _BiSetMatrixValuesIterative(_destination, values);

    if (status != BI_STATUS_SUCCESS)
        return status;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}
