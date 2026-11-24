#ifndef DS_DEQUE_H
#define DS_DEQUE_H

#include "../core/ds-types.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct deque_node deque_node_t;
typedef struct ds_deque deque_t;

// Lifecycle
deque_t *deque_create(free_fn free_fn);
void deque_destroy(deque_t *deque);
void deque_clear(deque_t *deque);

// Capacity & Info
size_t deque_size(const deque_t *deque);
bool deque_is_empty(const deque_t *deque);

// Modifiers
status_t deque_push_front(deque_t *deque, void *item);
status_t deque_pop_front(deque_t *deque, void **out_item);

status_t deque_push_rear(deque_t *deque, void *item);
status_t deque_pop_rear(deque_t *deque, void **out_item);

// Access
status_t deque_peek_front(const deque_t *deque, void **out_item);
status_t deque_peek_rear(const deque_t *deque, void **out_item);
bool deque_contains(const deque_t *deque, const void *item, cmp_fn cmp);

#endif
