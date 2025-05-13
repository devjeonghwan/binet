#include "binet/core/string.h"
#include "binet/api.h"
#include "binet/api_internal.h"

typedef struct
{
    size_t length;
    char*  data;
} _BiString;

#define BI_STRING_MAX_LENGTH (1024 * 1024)

BiStatus BiCreateString(BiString** string, const char* source)
{
    if (string == NULL || source == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    size_t length = 0;
    while (source[length] != '\0')
    {
        if (length >= BI_STRING_MAX_LENGTH)
            BI_RETURN_STATUS(BI_STATUS_ERROR_OVERFLOW);
        length++;
    }

    _BiString* newString = _BiAllocation(sizeof(_BiString));
    if (newString == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);

    newString->data = (char*)_BiAllocation(length + 1);
    if (newString->data == NULL)
    {
        _BiDeallocation(newString);
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);
    }

    for (size_t i          = 0; i <= length; i++)
        newString->data[i] = source[i];

    newString->length = length;

    *string = (BiString*)newString;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiDestroyString(BiString** string)
{
    if (string == NULL || *string == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    _BiString* _string = (_BiString*)*string;

    if (_string->data != NULL)
    {
        _BiDeallocation(_string->data);
        _string->data = NULL;
    }

    _BiDeallocation(_string);
    *string = NULL;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiConcatString(BiString** out, const BiString* string1, const BiString* string2)
{
    if (out == NULL || string1 == NULL || string2 == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiString* _string1 = (_BiString*)string1;
    const _BiString* _string2 = (_BiString*)string2;

    const size_t newLength = _string1->length + _string2->length;

    _BiString* newString = _BiAllocation(sizeof(_BiString));
    if (newString == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);

    newString->data = (char*)_BiAllocation(newLength + 1);
    if (newString->data == NULL)
    {
        _BiDeallocation(newString);
        BI_RETURN_STATUS(BI_STATUS_ERROR_ALLOCATION);
    }

    for (size_t i          = 0; i < _string1->length; i++)
        newString->data[i] = _string1->data[i];

    for (size_t i                             = 0; i < _string2->length; i++)
        newString->data[_string1->length + i] = _string2->data[i];

    newString->data[newLength] = '\0';
    newString->length          = newLength;

    *out = (BiString*)newString;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}

BiStatus BiGetStringData(const char** out, const BiString* string)
{
    if (out == NULL || string == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    const _BiString* _string = (const _BiString*)string;

    if (_string->data == NULL)
        BI_RETURN_STATUS(BI_STATUS_ERROR_INVALID_ARGUMENT);

    *out = _string->data;

    BI_RETURN_STATUS(BI_STATUS_SUCCESS);
}
