#ifndef DS_RING_BUFFER_H
#define DS_RING_BUFFER_H

#include "../core/ds-types.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct ds_ring_buffer ring_buffer_t;

// Lifecycle
ring_buffer_t *ring_buffer_create(size_t initial_cap, free_fn free_fn);
void ring_buffer_destroy(ring_buffer_t *buffer);
void ring_buffer_clear(ring_buffer_t *buffer);

// Capacity & Info
size_t ring_buffer_size(const ring_buffer_t *buffer);
size_t ring_buffer_capacity(const ring_buffer_t *buffer);
bool ring_buffer_is_empty(const ring_buffer_t *buffer);
bool ring_buffer_is_full(const ring_buffer_t *buffer);

// Modifiers
status_t ring_buffer_enqueue(ring_buffer_t *buffer, void *item);
status_t ring_buffer_dequeue(ring_buffer_t *buffer, void **out_item);

// Access
status_t ring_buffer_peek_front(const ring_buffer_t *buffer, void **out_item);
status_t ring_buffer_peek_rear(const ring_buffer_t *buffer, void **out_item);
bool ring_buffer_contains(const ring_buffer_t *buffer, const void *item, cmp_fn cmp);

#endif
