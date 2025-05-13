#ifndef API_INTERNAL_H
#define API_INTERNAL_H

#include "binet/api.h"

#define BI_RETURN_STATUS(status) do { _BiSetLastError(status, __FILE__, __LINE__, __func__); return status; } while(0)

#define BI_SET_STATUS(status) do { _BiSetLastError(status, __FILE__, __LINE__, __func__); } while(0)

void _BiSetLastError(BiStatus status, const char* file, uint32_t line, const char* function);

void* _BiAllocation(size_t size);

void _BiDeallocation(void* pointer);

#endif //API_INTERNAL_H
