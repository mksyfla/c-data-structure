#ifndef DS_ERROR_H
#define DS_ERROR_H

typedef enum {
    DS_OK = 0,
    DS_ERR_ALLOC,
    DS_ERR_OUT_OF_BOUNDS,
    DS_ERR_EMPTY,
    DS_ERR_INVALID_ARG
} status_t;

#endif
