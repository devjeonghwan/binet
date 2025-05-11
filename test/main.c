#include <stdio.h>
#include <stdlib.h>

#include <binet/api.h>
#include <binet/core/matrix.h>
#include <binet/core/string.h>

#define BINET_CHECK(status) { if (status != BINET_STATUS_SUCCESS) { BiNetPrintLastError(); exit(1); } }

int main()
{
    BINET_CHECK(BiNetInitialize(BINET_LOG_LEVEL_DEBUG, NULL));

    BiNetMatrix* operand1 = NULL;
    BINET_CHECK(BiNetCreateMatrix(&operand1, (BiNetSize[]){64}, 1, BINET_DATA_TYPE_BINARY));

    BiNetMatrix* operand2 = NULL;
    BINET_CHECK(BiNetCreateMatrix(&operand2, (BiNetSize[]){64}, 1, BINET_DATA_TYPE_BINARY));

    BiNetString* string = NULL;
    BINET_CHECK(BiNetGetMatrixString(&string, operand1, false));

    const char* stringData = NULL;
    BINET_CHECK(BiNetGetStringData(&stringData, string));

    printf("%s\r\n", stringData);

    BINET_CHECK(BiNetFinalize());

    return 0;
}
