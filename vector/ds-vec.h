#ifndef DS_VEC_H
#define DS_VEC_H

#include "../errors/ds-error.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct ds_vec vec_t;
typedef void (*free_fn)(void *ptr);

// Lifecycle
vec_t *vec_create(size_t initial_cap, free_fn free_fn);
void vec_destroy(vec_t *vec);
void vec_clear(vec_t *vec);

// Capacity & Info
size_t vec_size(const vec_t *vec);
size_t vec_capacity(const vec_t *vec);
bool vec_is_empty(const vec_t *vec);

// Modifiers
status_t vec_push(vec_t *vec, void *item);
status_t vec_pop(vec_t *vec, void **out_item);
status_t vec_insert(vec_t *vec, size_t index, void *item);
status_t vec_remove(vec_t *vec, size_t index, void **out_item);

// Access
status_t vec_get(const vec_t *vec, size_t index, void **out_item);
status_t vec_set(vec_t *vec, size_t index, void *item);

#endif
