#include "binet/core/matrix/operator_bitwise.h"
#include "binet/core/matrix_internal.h"
#include "binet/api_internal.h"

BiStatus BiComputeMatrixBitwiseAnd(BiMatrix* destination, const BiMatrix* operand1, const BiMatrix* operand2)
{
    if (destination == NULL || operand1 == NULL || operand2 == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiMatrix* _destination = (_BiMatrix*)destination;
    const _BiMatrix* _operand1    = (_BiMatrix*)operand1;
    const _BiMatrix* _operand2    = (_BiMatrix*)operand2;

    if (BiIsFloatingPoint(_operand1->type) || BiIsFloatingPoint(_operand2->type))
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_DATA_TYPE);

    if (_operand1->type != _operand2->type || _destination->type != _operand1->type)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_DATA_TYPE);

    if (_operand1->rank != _operand2->rank || _operand1->rank != _destination->rank)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_SHAPE);

    const BiSize destinationShape[BI_MAX_RANK];

    if (!_BiGetBroadcastShape(destinationShape, _operand1->shape, _operand2->shape, _operand1->rank))
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_SHAPE);

    if (!_BiCompareShape(_destination->shape, destinationShape, _destination->rank))
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_SHAPE);

    const BiRank rank = _destination->rank;

    size_t   indices[BI_MAX_RANK]            = {0};
    BiOffset offsetsOperand1[BI_MAX_RANK]    = {0};
    BiOffset offsetsOperand2[BI_MAX_RANK]    = {0};
    BiOffset offsetsDestination[BI_MAX_RANK] = {0};
    size_t   depth                           = 0;

    // Currently, this SIMD acceleration code condition is too strict and needs to be improved later.
    while (depth < (size_t)rank)
    {
        if (indices[depth] < destinationShape[depth])
        {
            const BiOffset index                 = (BiOffset)indices[depth];
            const BiOffset nextOffsetOperand1    = offsetsOperand1[depth] + (_operand1->shape[depth] == 1 ? 0 : index) * _operand1->strides[depth];
            const BiOffset nextOffsetOperand2    = offsetsOperand2[depth] + (_operand2->shape[depth] == 1 ? 0 : index) * _operand2->strides[depth];
            const BiOffset nextOffsetDestination = offsetsDestination[depth] + index * _destination->strides[depth];

            if (depth + 1 == (size_t)rank)
            {
                switch (_operand1->type)
                {
                case BI_DATA_TYPE_BINARY:
                    {
                        BiUInt8*       destinationData = _destination->data;
                        const BiUInt8* operand1Data    = _operand1->data;
                        const BiUInt8* operand2Data    = _operand2->data;

                        const BiOffset byteIndex = nextOffsetDestination / 8;
                        const BiOffset bitIndex  = nextOffsetDestination % 8;
                        const BiUInt8  bit1      = (operand1Data[nextOffsetOperand1 / 8] >> (nextOffsetOperand1 % 8)) & 1;
                        const BiUInt8  bit2      = (operand2Data[nextOffsetOperand2 / 8] >> (nextOffsetOperand2 % 8)) & 1;
                        const BiUInt8  resultBit = bit1 & bit2;

                        if (resultBit)
                            destinationData[byteIndex] |= (1 << bitIndex);
                        else
                            destinationData[byteIndex] &= ~(1 << bitIndex);

                        break;
                    }
                case BI_DATA_TYPE_INT16:
                    {
                        BiInt16*       destinationData = _destination->data;
                        const BiInt16* operand1Data    = _operand1->data;
                        const BiInt16* operand2Data    = _operand2->data;

                        destinationData[nextOffsetDestination] = operand1Data[nextOffsetOperand1] & operand2Data[nextOffsetOperand2];
                        break;
                    }
                case BI_DATA_TYPE_UINT16:
                    {
                        BiUInt16*       destinationData = _destination->data;
                        const BiUInt16* operand1Data    = _operand1->data;
                        const BiUInt16* operand2Data    = _operand2->data;

                        destinationData[nextOffsetDestination] = operand1Data[nextOffsetOperand1] & operand2Data[nextOffsetOperand2];
                        break;
                    }
                default:
                    BI_RETURN_STATUS(BI_STATUS_ERROR_UNSUPPORTED_DATA_TYPE);
                }

                indices[depth]++;
            }
            else
            {
                depth++;
                indices[depth]            = 0;
                offsetsOperand1[depth]    = nextOffsetOperand1;
                offsetsOperand2[depth]    = nextOffsetOperand2;
                offsetsDestination[depth] = nextOffsetDestination;
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

BiStatus BiComputeMatrixBitwiseOr(BiMatrix* destination, const BiMatrix* operand1, const BiMatrix* operand2)
{
    BI_RETURN_STATUS(BI_STATUS_ERROR_NOT_IMPLEMENTED);
}

BiStatus BiComputeMatrixBitwiseXor(BiMatrix* destination, const BiMatrix* operand1, const BiMatrix* operand2)
{
    BI_RETURN_STATUS(BI_STATUS_ERROR_NOT_IMPLEMENTED);
}

BiStatus BiComputeMatrixBitwiseNot(BiMatrix* destination, const BiMatrix* operand)
{
    BI_RETURN_STATUS(BI_STATUS_ERROR_NOT_IMPLEMENTED);
}
