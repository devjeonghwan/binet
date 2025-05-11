#include "binet/core/string.h"
#include "binet/api.h"
#include "binet/api_internal.h"

typedef struct
{
    char* data;
    size_t length;
} _BiNetString;

#define BINET_STRING_MAX_LENGTH (1024 * 1024)

BiNetStatus BiNetCreateString(BiNetString** string, const char* source)
{
    if (string == NULL || source == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    size_t length = 0;
    while (source[length] != '\0')
    {
        if (length >= BINET_STRING_MAX_LENGTH)
            BINET_RETURN_STATUS(BINET_STATUS_ERROR_OVERFLOW);
        length++;
    }

    _BiNetString* newString = (_BiNetString*)_BiNetAllocation(sizeof(_BiNetString));
    if (newString == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);

    newString->data = (char*)_BiNetAllocation(length + 1); // +1 for null-terminator
    if (newString->data == NULL)
    {
        _BiNetDeallocation(newString);
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);
    }

    for (size_t i = 0; i <= length; i++) // include null-terminator
        newString->data[i] = source[i];

    newString->length = length;

    *string = (BiNetString*)newString;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus BiNetDestroyString(BiNetString** string)
{
    if (string == NULL || *string == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    _BiNetString* _string = (_BiNetString*)*string;

    if (_string->data != NULL)
    {
        _BiNetDeallocation(_string->data);
        _string->data = NULL;
    }

    _BiNetDeallocation(_string);
    *string = NULL;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus BiNetConcatString(BiNetString** out, const BiNetString* string1, const BiNetString* string2)
{
    if (out == NULL || string1 == NULL || string2 == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiNetString* _string1 = (_BiNetString*)string1;
    const _BiNetString* _string2 = (_BiNetString*)string2;

    const size_t newLength = _string1->length + _string2->length;

    _BiNetString* newString = (_BiNetString*)_BiNetAllocation(sizeof(_BiNetString));
    if (newString == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);

    newString->data = (char*)_BiNetAllocation(newLength + 1); // +1 for null-terminator
    if (newString->data == NULL)
    {
        _BiNetDeallocation(newString);
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_ALLOCATION);
    }

    for (size_t i = 0; i < _string1->length; i++)
        newString->data[i] = _string1->data[i];

    for (size_t i = 0; i < _string2->length; i++)
        newString->data[_string1->length + i] = _string2->data[i];

    newString->data[newLength] = '\0';
    newString->length = newLength;

    *out = (BiNetString*)newString;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}

BiNetStatus BiNetGetStringData(const char** out, const BiNetString* string)
{
    if (out == NULL || string == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiNetString* _string = (const _BiNetString*)string;

    if (_string->data == NULL)
        BINET_RETURN_STATUS(BINET_STATUS_ERROR_INVALID_ARGUMENT);

    *out = _string->data;

    BINET_RETURN_STATUS(BINET_STATUS_SUCCESS);
}