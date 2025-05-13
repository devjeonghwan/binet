#include <stdio.h>
#include <stdlib.h>

#include <binet/api.h>
#include <binet/core/string.h>
#include <binet/core/matrix.h>
#include <binet/core/matrix/operator_set.h>
#include <binet/core/matrix/operator_bitwise.h>

#define BI_CHECK(status) { if (status != BI_STATUS_SUCCESS) { BiPrintLastError(); exit(1); } }

int main()
{
    BI_CHECK(BiInitialize(BI_LOG_LEVEL_DEBUG, NULL));

    BiMatrix* operand1 = NULL;
    BiMatrix* operand2 = NULL;

    BI_CHECK(BiCreateMatrix(&operand1, (BiSize[]){2,2}, 2, BI_DATA_TYPE_BINARY));
    BI_CHECK(BiCreateMatrix(&operand2, (BiSize[]){1,2}, 2, BI_DATA_TYPE_BINARY));

    BI_CHECK(BiSetMatrixBinaryValues(operand1, (BiBool[]){1, 1, 1, 1}));
    BI_CHECK(BiSetMatrixBinaryValues(operand2, (BiBool[]){1, 0}));

    BiMatrix* result = NULL;

    BI_CHECK(BiCreateMatrix(&result, (BiSize[]){2,2}, 2, BI_DATA_TYPE_BINARY));
    BI_CHECK(BiComputeMatrixBitwiseAnd(result, operand1, operand2));

    BiString*   string     = NULL;
    const char* stringData = NULL;

    BI_CHECK(BiGetMatrixString(&string, result, false));
    BI_CHECK(BiGetStringData(&stringData, string));

    printf("%s\r\n", stringData);

    BI_CHECK(BiFinalize());

    return 0;
}
