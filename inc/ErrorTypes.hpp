#ifndef ERROR_TYPES_H_
#define ERROR_TYPES_H_

#define FREE(ptr) \
    free(ptr);    \
    ptr = NULL

enum ErrorType {
    SUCCESS          = 0,
    ALLOCATION_ERROR = 1,
    NULL_PTR_ERROR   = 1 << 1
};

#endif
