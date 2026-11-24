#ifndef DS_VECTOR_H
#define DS_VECTOR_H

#include "../core/ds-types.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct ds_vector vector_t;

// Lifecycle
vector_t *vector_create(size_t initial_cap, free_fn free_fn);
void vector_destroy(vector_t *vector);
void vector_clear(vector_t *vector);

// Capacity & Info
size_t vector_size(const vector_t *vector);
size_t vector_capacity(const vector_t *vector);
bool vector_is_empty(const vector_t *vector);

// Modifiers
status_t vector_push(vector_t *vector, void *item);
status_t vector_pop(vector_t *vector, void **out_item);
status_t vector_insert(vector_t *vector, size_t index, void *item);
status_t vector_remove(vector_t *vector, size_t index, void **out_item);
status_t vector_set(vector_t *vector, size_t index, void *item);

// Access
status_t vector_get(const vector_t *vector, size_t index, void **out_item);
bool vector_contains(const vector_t *vector, const void *item, cmp_fn cmp);
size_t vector_index_of(const vector_t *vector, const void *item, cmp_fn cmp);

#endif
