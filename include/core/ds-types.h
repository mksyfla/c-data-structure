#ifndef DS_TYPES_H
#define DS_TYPES_H

typedef enum {
    DS_OK = 0,
    DS_ERR_ALLOC,
    DS_ERR_OUT_OF_BOUNDS,
    DS_ERR_EMPTY,
    DS_ERR_OVERFLOW,
    DS_ERR_INVALID_ARG
} status_t;

typedef void (*free_fn)(void *ptr);

typedef int (*cmp_fn)(const void *a, const void *b);

#endif
